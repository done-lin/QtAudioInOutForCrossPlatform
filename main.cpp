#include "mainaudiowindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainAudioWindow w;
    w.show();

    return a.exec();
}
