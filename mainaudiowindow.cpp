#include "mainaudiowindow.h"
#include <QAudioDeviceInfo>
#include <QAudioOutput>

MainAudioWindow::MainAudioWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_AudioInOutDevice = new AudioOutDevWidget(this, 22050, 1, 16, 100, false,
                                               QAudioFormat::SignedInt, QAudioFormat::LittleEndian);
    m_AudioInputDevice = new audioInputDevWidget(this, 22050, 1, 16, 100,
                                                 QAudioFormat::SignedInt, QAudioFormat::LittleEndian);
    connect(m_AudioInputDevice, SIGNAL(signal_finished_reading_from_microphone(QByteArray)),
            m_AudioInOutDevice, SLOT(slot_audio_output_get_data(QByteArray)));

}

MainAudioWindow::~MainAudioWindow()
{

}
