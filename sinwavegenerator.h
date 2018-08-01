#ifndef SINWAVEGENERATOR_H
#define SINWAVEGENERATOR_H
#include <QAudio>
#include <QAudioFormat>
#include <QIODevice>
#include <qmath.h>

class SinWaveGenerator : public QIODevice
{
    Q_OBJECT

public:
    SinWaveGenerator(const QAudioFormat &format, qint64 durationUs, int sampleRate, QObject *parent);
    ~SinWaveGenerator();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;

private:
    void generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate);

private:
    qint64 m_pos;
    QByteArray m_buffer;
};

#endif // SINWAVEGENERATOR_H
