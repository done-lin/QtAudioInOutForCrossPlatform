#include "mainaudiowindow.h"
#include <QAudioDeviceInfo>
#include <QAudioOutput>

MainAudioWindow::MainAudioWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_AudioInOutDevice = new AudioInOutDevice(this, 22050, 1, 16, 100, true, QAudioFormat::SignedInt, QAudioFormat::LittleEndian);
}

MainAudioWindow::~MainAudioWindow()
{

}