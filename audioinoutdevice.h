#ifndef AUDIOINOUTDEVICE_H
#define AUDIOINOUTDEVICE_H

#include <QObject>
#include <QWidget>
#include <QAudio>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QTimer>
#include <QByteArray>
#include "sinwavegenerator.h"

class AudioOutDevWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudioOutDevWidget(QWidget *parent=0,  int sampleRate=44100, int channelCount=1, int SampleSize=16, int volume=100, bool usingTest=false,
            QAudioFormat::SampleType sampleType=QAudioFormat::SignedInt, QAudioFormat::Endian byteOrder=QAudioFormat::LittleEndian);


    void set_sample_rate(int sampleRate);       //8000, 11025, 16000, 22050, 44100 and 48000
    void set_channel_count(int channelCount);   //how many channels for this audio devices!
    void set_sample_size(int sampleSize);       //8 bit? 16 bit? 32bit?
    void set_codec(const QString &codec);       //"audio/pcm"
    void create_testing_wave_generator(qint64 durationInUs, int soundFrequency);
    void start_testing_wave_generator(void);
    void create_sound_outputer(void);
    void suspend_output();
    void resume_output();

    QAudioFormat m_format;
    SinWaveGenerator *m_generator;
    QByteArray m_buffer;

private:
    bool m_testFlag;
    QTimer *m_pushTimer;
    QAudioOutput *m_audioOutput;
    QAudioInput *m_audioInput;
    QAudioDeviceInfo m_device;
    QIODevice *m_output; // not owned
public slots:
    void slot_volume_changed(int volume);
private slots:
    void slot_audio_output_push_timer_expired();
};

#endif // AUDIOINOUTDEVICE_H
