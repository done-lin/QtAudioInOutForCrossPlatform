#include "mainaudiowindow.h"
#include <QAudioDeviceInfo>
#include <QAudioOutput>

MainAudioWindow::MainAudioWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSharedPointer <QByteArray> aSharedPtr;
    aSharedPtr = QSharedPointer <QByteArray>(new QByteArray());

    m_AudioInputDevice = new audioInputDevWidget(this, 22050, 1, 16, 100,
                                                 QAudioFormat::SignedInt, QAudioFormat::LittleEndian, aSharedPtr);

    m_AudioInOutDevice = new AudioOutDevWidget(this, 22050, 1, 16, 100, false,
                                               QAudioFormat::SignedInt, QAudioFormat::LittleEndian, aSharedPtr);

    m_AudioInOutDevice->m_sharedBuffer->resize(m_AudioInputDevice->audio_buffer_length());
    m_AudioInOutDevice->m_sharedBuffer->fill(0);

    connect(m_AudioInputDevice, SIGNAL(signal_finished_reading_from_microphone()),
            m_AudioInOutDevice, SLOT(slot_audio_output_get_data()));

}

MainAudioWindow::~MainAudioWindow()
{

}
