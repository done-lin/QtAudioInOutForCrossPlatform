#include "audioinfo.h"
#include <QtEndian>
#include <QTime>
#include <QDebug>

AudioInfo::AudioInfo(const QAudioFormat &format, QObject *parent)
    :   QIODevice(parent)
    ,   m_format(format)
    ,   m_maxAmplitude(0)
    ,   m_level(0.0)
    ,   m_inBuffer(32768, 0)

{
    switch (m_format.sampleSize()) {
    case 8:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 32767;
            break;
        default:
            break;
        }
        break;

    case 32:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 0xffffffff;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 0x7fffffff;
            break;
        case QAudioFormat::Float:
            m_maxAmplitude = 0x7fffffff; // Kind of
        default:
            break;
        }
        break;

    default:
        break;
    }
    m_audioBufferLength = calculate_audio_length(m_format, 12*1000000);
    qDebug("%s[%d]: m_audioBufferLength: %d", __FUNCTION__, __LINE__, m_audioBufferLength);
    m_inBuffer.resize(m_audioBufferLength);
    m_inBuffer.fill(0);
}

AudioInfo::~AudioInfo()
{
}

void AudioInfo::start()
{
    open(QIODevice::WriteOnly);
}

void AudioInfo::stop()
{
    close();
}

//qint64 AudioInfo::readData(char *data, qint64 maxlen)
//{
//    Q_UNUSED(data)
//    Q_UNUSED(maxlen)

//    return 0;
//}

//qint64 AudioInfo::writeData(const char *data, qint64 len)
//{
//    if (m_maxAmplitude) {
//        Q_ASSERT(m_format.sampleSize() % 8 == 0);
//        const int channelBytes = m_format.sampleSize() / 8;
//        const int sampleBytes = m_format.channelCount() * channelBytes;
//        Q_ASSERT(len % sampleBytes == 0);
//        const int numSamples = len / sampleBytes;

//        quint32 maxValue = 0;
//        m_inBuffer.clear();
//        m_inBuffer.resize(len);
//        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
//        unsigned char *bufferPtr = reinterpret_cast<unsigned char *>(m_inBuffer.data());

//        for (int i = 0; i < numSamples; ++i) {
//            for (int j = 0; j < m_format.channelCount(); ++j) {
//                quint32 value = 0;

//                if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
//                    value = *reinterpret_cast<const quint8*>(ptr);
//                    *reinterpret_cast<quint8*>(bufferPtr) = *reinterpret_cast<const quint8*>(ptr);
//                } else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::SignedInt) {
//                    value = qAbs(*reinterpret_cast<const qint8*>(ptr));
//                    *reinterpret_cast<qint8*>(bufferPtr) = *reinterpret_cast<const qint8*>(ptr);
//                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
//                    if (m_format.byteOrder() == QAudioFormat::LittleEndian){
//                        value = qFromLittleEndian<quint16>(ptr);
//                    }else{
//                        value = qFromBigEndian<quint16>(ptr);
//                    }
//                    *reinterpret_cast<quint16*>(bufferPtr) = *reinterpret_cast<const quint16*>(ptr);
//                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::SignedInt) {
//                    if (m_format.byteOrder() == QAudioFormat::LittleEndian){
//                        value = qAbs(qFromLittleEndian<qint16>(ptr));
//                    }else{
//                        value = qAbs(qFromBigEndian<qint16>(ptr));
//                    }
//                    *reinterpret_cast<qint16*>(bufferPtr) = *reinterpret_cast<const qint16*>(ptr);
//                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
//                    if (m_format.byteOrder() == QAudioFormat::LittleEndian){
//                        value = qFromLittleEndian<quint32>(ptr);
//                    }else{
//                        value = qFromBigEndian<quint32>(ptr);
//                    }
//                    *reinterpret_cast<quint32*>(bufferPtr) = *reinterpret_cast<const quint32*>(ptr);
//                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::SignedInt) {
//                    if (m_format.byteOrder() == QAudioFormat::LittleEndian){
//                        value = qAbs(qFromLittleEndian<qint32>(ptr));
//                    }else{
//                        value = qAbs(qFromBigEndian<qint32>(ptr));
//                    }
//                    *reinterpret_cast<qint32*>(bufferPtr) = *reinterpret_cast<const qint32*>(ptr);
//                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::Float) {
//                    value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff); // assumes 0-1.0
//                    *reinterpret_cast<float*>(bufferPtr) = *reinterpret_cast<const float*>(ptr);
//                }

//                //qDebug("%s[%d]: value:%u", __FUNCTION__, __LINE__, value);
//                *bufferPtr = qrand()%256;//add for test
//                maxValue = qMax(value, maxValue);
//                ptr += channelBytes;
//                bufferPtr += channelBytes;
//            }
//        }

//        emit signal_sound_data_buffer(m_inBuffer);
//        maxValue = qMin(maxValue, m_maxAmplitude);
//        m_level = qreal(maxValue) / m_maxAmplitude;
//    }

//    return len;
//}


inline qint64 AudioInfo::audio_length()
{
    return m_audioBufferLength;
}

qint64 AudioInfo::calculate_audio_length(const QAudioFormat &format, qint64 microSeconds)
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
