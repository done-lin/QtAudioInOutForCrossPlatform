#ifndef MAINAUDIOWINDOW_H
#define MAINAUDIOWINDOW_H

#include <QMainWindow>
#include "audioinoutdevice.h"

class MainAudioWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainAudioWindow(QWidget *parent = 0);
    ~MainAudioWindow();

    AudioInOutDevice *m_AudioInOutDevice;
};

#endif // MAINAUDIOWINDOW_H
