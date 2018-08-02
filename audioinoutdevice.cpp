#include "audioinoutdevice.h"
#include <QDebug>

#define DEF_DURATIONSECONDS 1
#define DEF_WAVE_FREQUENCY 400
#define DEF_USING_TEST

const int BufferSize      = 32768;

AudioOutDevWidget::AudioOutDevWidget(QWidget *parent, int sampleRate, int channelCount, int SampleSize, int volume, bool usingTest,
                                   QAudioFormat::SampleType sampleType, QAudioFormat::Endian byteOrder)
        : QWidget(parent)
        ,   m_device(QAudioDeviceInfo::defaultOutputDevice())
        ,   m_generator(0)
        ,   m_audioOutput(0)
        ,   m_audioInput(0)
        ,   m_output(0)
        ,   m_buffer(BufferSize, 0)
{
    m_pushTimer = new QTimer(this);
    this->setParent(parent);
    m_testFlag = usingTest;
    //connect(m_pushTimer, SIGNAL(timeout()), SLOT(slot_audio_output_push_timer_expired()));

    m_format.setSampleRate(sampleRate);
    m_format.setChannelCount(channelCount);
    m_format.setSampleSize(SampleSize);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(byteOrder);
    m_format.setSampleType(sampleType);

    QAudioDeviceInfo info(m_device);
    if (!info.isFormatSupported(m_format)) {
        qWarning() << "OUTPUT: Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }

    if(m_testFlag){
        create_testing_wave_generator(DEF_DURATIONSECONDS*1000000, DEF_WAVE_FREQUENCY);
    }

    create_sound_outputer();
    slot_volume_changed(volume);

    if(m_testFlag){
        start_testing_wave_generator();
    }

    m_output = m_audioOutput->start();
    //m_pushTimer->start(20);

}


void AudioOutDevWidget::set_sample_rate(int sampleRate)
{
    m_format.setSampleRate(sampleRate);
}

void AudioOutDevWidget::set_channel_count(int channelCount)
{
    m_format.setChannelCount(channelCount);
}

void AudioOutDevWidget::set_sample_size(int sampleSize)
{
    m_format.setSampleSize(sampleSize);
}

void AudioOutDevWidget::set_codec(const QString &codec)
{
    m_format.setCodec(codec);
}

void AudioOutDevWidget::create_testing_wave_generator(qint64 durationInUs, int soundFrequency)
{
    if (m_generator)
        delete m_generator;
    m_generator = new SinWaveGenerator(m_format, durationInUs, soundFrequency, this);
}

void AudioOutDevWidget::start_testing_wave_generator()
{
    m_generator->start();
    //m_audioOutput->start(m_generator);
}

void AudioOutDevWidget::create_sound_outputer()
{
    delete m_audioOutput;
    m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);
}

void AudioOutDevWidget::suspend_output()
{
    m_audioOutput->suspend();
}

void AudioOutDevWidget::resume_output()
{
    m_audioOutput->resume();
}

void AudioOutDevWidget::slot_volume_changed(int volume)
{
    if (m_audioOutput) {
//        qreal linearVolume =  QAudio::convertVolume(volume / qreal(100),
//                                                    QAudio::LogarithmicVolumeScale,
//                                                    QAudio::LinearVolumeScale);
        qreal linearVolume =  volume / qreal(100.0f);

        m_audioOutput->setVolume(linearVolume);
    }
}

void AudioOutDevWidget::slot_audio_output_push_timer_expired()
{
    if (m_audioOutput && m_audioOutput->state() != QAudio::StoppedState) {
        int chunks = m_audioOutput->bytesFree()/m_audioOutput->periodSize();
        while (chunks) {
            if(m_testFlag){
                const qint64 len = m_generator->read(m_buffer.data(), m_audioOutput->periodSize());
                if (len)
                   m_output->write(m_buffer.data(), len);
                if (len != m_audioOutput->periodSize())
                   break;
            }else{
                qint64 len;
                //const qint64 len = someIODevice_read(m_buffer.data(), m_audioOutput->periodSize());
                if (len)
                   m_output->write(m_buffer.data(), len);
                if (len != m_audioOutput->periodSize())
                   break;
            }
           --chunks;
        }
    }
}

void AudioOutDevWidget::slot_audio_output_get_data(QByteArray array)
{
    if (m_audioOutput && m_audioOutput->state() != QAudio::StoppedState) {
        qDebug("%s[%d]: bytesFree:%d, periodSize:%d, array.size:%d", __FUNCTION__, __LINE__,
               m_audioOutput->bytesFree(),m_audioOutput->periodSize(), array.size());

            if ((m_audioOutput->bytesFree()<m_audioOutput->periodSize()) ||
                    (array.size() < m_audioOutput->periodSize())){
                qDebug("%s[%d]: not enough", __FUNCTION__, __LINE__);
                return;
            }
            m_output->write(array.data(), m_audioOutput->periodSize());

    }
}
