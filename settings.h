#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>
#include <QFile>
namespace Ui {
class settings;
}

class settings : public QMainWindow
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = 0);
    ~settings();

private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::settings *ui;
    QString configFile = "config.dat";
    // defaul setting parameters
    QString timerPeriod = "30";
    QString cigarettesCost = "15";
    QString cigarettesCount = "20";
    QString cannabisCost = "7";
    QString cannabisCount = "5";
    QString messageEnable = "1";
    QString soundEnable = "1";
};

#endif // SETTINGS_H
