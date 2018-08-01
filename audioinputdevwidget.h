#ifndef AUDIOINPUTDEVWIDGET_H
#define AUDIOINPUTDEVWIDGET_H

#include <QObject>
#include <QWidget>
#include <QAudio>
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QByteArray>
#include "audioinfo.h"

class audioInputDevWidget : public QWidget
{
    Q_OBJECT
public:
    explicit audioInputDevWidget(QWidget *parent=0,  int sampleRate=44100, int channelCount=1, int SampleSize=16, int volume=100,
                                 QAudioFormat::SampleType sampleType=QAudioFormat::SignedInt, QAudioFormat::Endian byteOrder=QAudioFormat::LittleEndian);


    void set_sample_rate(int sampleRate);       //8000, 11025, 16000, 22050, 44100 and 48000
    void set_channel_count(int channelCount);   //how many channels for this audio devices!
    void set_sample_size(int sampleSize);       //8 bit? 16 bit? 32bit?
    void set_codec(const QString &codec);       //"audio/pcm"
    void input_suspend();
    void input_resume();
    void createAudioInput();
private:
    QAudioDeviceInfo m_device;
    AudioInfo *m_audioInfo;
    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QIODevice *m_input;
    QByteArray m_buffer;

signals:

public slots:
    void slot_volume_changed(int volume);
    void slot_readMore();
};

#endif // AUDIOINPUTDEVWIDGET_H
