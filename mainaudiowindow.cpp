#include "mainaudiowindow.h"
#include <QAudioDeviceInfo>
#include <QAudioOutput>

MainAudioWindow::MainAudioWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_AudioInOutDevice = new AudioOutDevWidget(this, 22050, 1, 16, 100, true,
                                               QAudioFormat::SignedInt, QAudioFormat::LittleEndian);
    m_AudioInputDevice = new audioInputDevWidget(this, 22050, 1, 16, 100,
                                                 QAudioFormat::SignedInt, QAudioFormat::LittleEndian);
    connect(m_AudioInputDevice->m_audioInfo, SIGNAL(signal_sound_data_buffer(QByteArray)),
            m_AudioInOutDevice, SLOT(slot_audio_output_get_data(QByteArray)));

}

MainAudioWindow::~MainAudioWindow()
{

}
