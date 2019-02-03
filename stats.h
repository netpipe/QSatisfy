#ifndef STATS_H
#define STATS_H

#include <QMainWindow>

#include <QtCharts>
using namespace QtCharts;

namespace Ui {
class stats;
}

class stats : public QMainWindow
{
    Q_OBJECT

public:
    explicit stats(QWidget *parent = 0);
    ~stats();
    void chartMonthDisplay(QString selectedYear);
    void chartYearDisplay();
    void chartDayDisplay(QString selectedYear , QString selectedMonth);

    void textStatsDisplay();
    
private slots:

    void on_btnReset_clicked();

    void on_comboYear_Month_currentIndexChanged(int index);

    void on_comboYear_Day_currentIndexChanged(int index);

    void on_comboMonth_Day_currentIndexChanged(int index);


private:
    Ui::stats *ui;
    QString dataFile = "data.dat";
    QVector<QStringList> smokeData;
    QVector<QStringList> smokeCanData;
    QVector<QStringList> smokeCigData;
    QVector<QStringList> skipData;
    QStringList years;
    QMap<QString , QString> mapMonth;
};

#endif // STATS_H
