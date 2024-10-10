#include "mainwindow.h"
#include <QApplication>

#ifdef __APPLE__
QString mediadir2 = "/Applications/QSatisfy.app/Contents/media/"; //change mediadir2 in main.cpp aswell if needed
#else
QString mediadir2 = "./media/"; //change mediadir2 in main.cpp aswell if needed
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(mediadir2 + "smoking.png"));
    a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    w.setWindowFlags( Qt::Dialog  | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
    w.show();

    return a.exec();
}
