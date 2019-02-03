#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "stats.h"
#include "settings.h"
#include <QMainWindow>

#include <QMenu>
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void showEvent(QShowEvent *ev);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showMessage();

private slots:
    void on_btnSmoked_clicked();

    void on_btnSkipped_clicked();

    void on_btnHide_clicked();

    void updateCheck();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void on_exit();

    void openStatsWindow();

    void openSettingsWindow();

    void showTime();
    void on_actionExit_triggered();

    void on_btnViewSetting_clicked();

    void on_actionSetting_triggered();

    void on_actionstats_triggered();

private:
    Ui::MainWindow *ui;
    QString dataFile = "data.dat";
    QString configFile = "config.dat";

    // defaul setting parameters
    QString timerPeriod = "30";

    QString cigarettesCost = "15";
    QString cigarettesCount = "20";
    QString cannabisCost = "1";
    QString cannabisCount = "1";
    QString messageEnable = "1";
    QString soundEnable = "1";
    QString backgroundImage = "";


    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    stats *statsWindow;
    settings *settingsWindow;

};

#endif // MAINWINDOW_H
