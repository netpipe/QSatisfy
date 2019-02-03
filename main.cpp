#include "mainwindow.h"
#include <QApplication>

QString mediadir2 = "./media/";


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
