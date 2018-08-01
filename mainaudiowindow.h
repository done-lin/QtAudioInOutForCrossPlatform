#ifndef MAINAUDIOWINDOW_H
#define MAINAUDIOWINDOW_H

#include <QMainWindow>
#include "audioinoutdevice.h"
#include "audioinputdevwidget.h"

class MainAudioWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainAudioWindow(QWidget *parent = 0);
    ~MainAudioWindow();

    AudioOutDevWidget *m_AudioInOutDevice;
    audioInputDevWidget *m_AudioInputDevice;
};

#endif // MAINAUDIOWINDOW_H
