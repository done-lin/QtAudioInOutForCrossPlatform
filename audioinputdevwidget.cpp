#include "audioinputdevwidget.h"
#include "audioinfo.h"
#include "qdebug.h"

const int BufferSize = 4096;

audioInputDevWidget::audioInputDevWidget(QWidget *parent, int sampleRate, int channelCount, int SampleSize, int volume,
                                         QAudioFormat::SampleType sampleType, QAudioFormat::Endian byteOrder)
    : QWidget(parent)
    ,   m_device(QAudioDeviceInfo::defaultInputDevice())
    ,   m_audioInfo(0)
    ,   m_audioInput(0)
    ,   m_input(0)
    ,   m_buffer(BufferSize, 0)
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
    qDebug("%s[%d]: ", __FUNCTION__, __LINE__);
    if (m_audioInfo)
        delete m_audioInfo;
    m_audioInfo  = new AudioInfo(m_format, this);
    qDebug("%s[%d]: ", __FUNCTION__, __LINE__);
    createAudioInput();
    qDebug("%s[%d]: ", __FUNCTION__, __LINE__);
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
    m_audioInput->suspend();
}

void audioInputDevWidget::createAudioInput()
{
    m_audioInput = new QAudioInput(m_device, m_format, this);
    m_audioInfo->start();
    m_audioInput->start(m_audioInfo);
}

void audioInputDevWidget::slot_volume_changed(int volume)
{
    m_audioInput->setVolume(volume/100.0f);
}

//void audioInputDevWidget::slot_readMore()
//{
//    if (!m_audioInput)
//        return;
//    qint64 len = m_audioInput->bytesReady();
//    if (len > BufferSize)
//        len = BufferSize;
//    qint64 l = m_input->read(m_buffer.data(), len);
//    if (l > 0)
//        m_audioInfo->write(m_buffer.constData(), l);
//}
