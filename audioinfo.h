#ifndef AUDIOINFO_H
#define AUDIOINFO_H

#include <QObject>
#include <QWidget>
#include <QAudio>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioDeviceInfo>

class AudioInfo : public QIODevice
{
    Q_OBJECT

public:
    AudioInfo(const QAudioFormat &format, QObject *parent);
    ~AudioInfo();

    void start();
    void stop();

    qreal level() const { return m_level; }

//    qint64 readData(char *data, qint64 maxlen);
//    qint64 writeData(const char *data, qint64 len);
    qint64 audio_length(void);
    QByteArray m_inBuffer;
private:
    const QAudioFormat m_format;
    quint32 m_maxAmplitude;
    qreal m_level; // 0.0 <= m_level <= 1.0
    qint64 m_audioBufferLength;

    qint64 calculate_audio_length(const QAudioFormat &format, qint64 microSeconds);
signals:
    void signal_sound_data_buffer(QByteArray array);
};

#endif // AUDIOINFO_H
