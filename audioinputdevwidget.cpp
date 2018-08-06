#include "audioinputdevwidget.h"
#include "audioinfo.h"
#include "qdebug.h"

const int BufferSize = 4096;

audioInputDevWidget::audioInputDevWidget(QWidget *parent, int sampleRate, int channelCount, int SampleSize, int volume,
                                         QAudioFormat::SampleType sampleType, QAudioFormat::Endian byteOrder)
    : QWidget(parent)
    ,   m_device(QAudioDeviceInfo::defaultInputDevice())
    ,   m_audioInput(0)
    ,   m_input(0)
    ,   m_buffer(BufferSize, 0)
    ,   m_bytesReady(0)
    ,   m_dataLengthRecord(0)
{
    this->setParent(parent);
    qDebug("%s[%d]: ", __FUNCTION__, __LINE__);
    m_format.setSampleRate(sampleRate);
    m_format.setChannelCount(channelCount);
    m_format.setSampleSize(SampleSize);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(byteOrder);
    m_format.setSampleType(sampleType);
    qDebug("%s[%d]: ", __FUNCTION__, __LINE__);

    QAudioDeviceInfo info(m_device);
    if (!info.isFormatSupported(m_format)) {
        qWarning() << "INPUT dev: Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }
    m_bufferLength = calculate_audio_length(m_format, 8*1000000);
    qDebug("%s[%d]: ", __FUNCTION__, __LINE__);
    createAudioInput();
    qDebug("%s[%d]: ", __FUNCTION__, __LINE__);
    start_recording();
    slot_volume_changed(volume);

}

void audioInputDevWidget::set_sample_rate(int sampleRate)
{
    m_format.setSampleRate(sampleRate);
}

void audioInputDevWidget::set_channel_count(int channelCount)
{
    m_format.setChannelCount(channelCount);
}

void audioInputDevWidget::set_sample_size(int sampleSize)
{
    m_format.setSampleSize(sampleSize);
}



void audioInputDevWidget::set_codec(const QString &codec)
{
    m_format.setCodec(codec);
}

void audioInputDevWidget::input_suspend()
{
    m_audioInput->suspend();
}

void audioInputDevWidget::input_resume()
{
    m_audioInput->resume();
}

void audioInputDevWidget::createAudioInput()
{
    m_audioInput = new QAudioInput(m_device, m_format, this);
}

void audioInputDevWidget::start_recording()
{
    if(m_audioInput){
        qDebug("%s[%d]: ", __FUNCTION__, __LINE__);
        m_dataLengthRecord = 0;
        m_buffer.resize(m_bufferLength);
        m_buffer.fill(0);
        m_input = m_audioInput->start();
        connect(m_input, SIGNAL(readyRead()),this,SLOT(slot_capture_data_from_mic()));
    }
    //
}

inline qint64 audioInputDevWidget::audio_buffer_length()
{
    return m_bufferLength;
}

qint64 audioInputDevWidget::calculate_audio_length(const QAudioFormat &format, qint64 microSeconds)
{
    qint64 result = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8))
            * microSeconds / 1000000;
    qDebug() << "result 001: " << result;
    result -= result % (format.channelCount() * format.sampleSize());
    qDebug("sampleRate:%d, channelCount:%d, sampleSize:%d",
            format.sampleRate(), format.channelCount(), format.sampleSize());
    qDebug() << "result 002: " << result;
    return result;
}
void audioInputDevWidget::slot_volume_changed(int volume)
{
    m_audioInput->setVolume(volume/100.0f);
}

void audioInputDevWidget::slot_capture_data_from_mic()
{
    const qint64 bytesReady = m_audioInput->bytesReady();
    m_bytesReady = bytesReady;
    const qint64 bytesSpace = m_buffer.size() - m_dataLengthRecord;  // what is m_dataLength?
    if (bytesReady > bytesSpace)
        qDebug("%s[%d]: Buffer is overflow", __FUNCTION__, __LINE__);
    const qint64 bytesToRead = qMin(bytesReady, bytesSpace);

    const qint64 bytesRead = m_input->read(m_buffer.data()+m_dataLengthRecord,bytesToRead);
    qDebug() <<"bytesRead: " << bytesRead;

//    for(int cnt=0; cnt<bytesToRead; cnt++){
//        m_buffer[cnt+(int)m_dataLengthRecord] = qrand()%256;
//    }

    if (bytesRead) {
        m_dataLengthRecord += bytesRead;
    }

    emit signal_finished_reading_from_microphone(m_buffer);

    if (m_buffer.size() >= m_dataLengthRecord) {
        m_dataLengthRecord = 0;
        qDebug() << "in capture Data buffer is full";
    }
}

