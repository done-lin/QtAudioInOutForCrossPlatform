#ifndef AUDIOINPUTDEVWIDGET_H
#define AUDIOINPUTDEVWIDGET_H

#include <QObject>
#include <QWidget>
#include <QAudio>
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QByteArray>
#include <QSharedPointer>

class audioInputDevWidget : public QWidget
{
    Q_OBJECT
public:
    explicit audioInputDevWidget(QWidget *parent=0,  int sampleRate=44100, int channelCount=1, int SampleSize=16, int volume=100,
                                 QAudioFormat::SampleType sampleType=QAudioFormat::SignedInt, QAudioFormat::Endian byteOrder=QAudioFormat::LittleEndian,
                                    const QSharedPointer <QByteArray> &m_audioDatSharedBuffer=0);


    void set_sample_rate(int sampleRate);       //8000, 11025, 16000, 22050, 44100 and 48000
    void set_channel_count(int channelCount);   //how many channels for this audio devices!
    void set_sample_size(int sampleSize);       //8 bit? 16 bit? 32bit?
    void set_codec(const QString &codec);       //"audio/pcm"
    void input_suspend();
    void input_resume();
    void createAudioInput();
    void start_recording(void);
    qint64 audio_buffer_length(void);
    qint64 calculate_audio_length(const QAudioFormat &format, qint64 microSeconds);

private:
    QAudioDeviceInfo m_device;

    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QIODevice *m_input;
    QSharedPointer <QByteArray> m_sharedBuffer;
    qint64 m_bytesReady;
    qint64 m_dataLengthRecord;
    qint64 m_bufferLength;
signals:
    void signal_finished_reading_from_microphone(void);

public slots:
    void slot_volume_changed(int volume);
    void slot_capture_data_from_mic(void);
//    void slot_readMore();
};

#endif // AUDIOINPUTDEVWIDGET_H
