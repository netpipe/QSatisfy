#include "stats.h"
#include "ui_stats.h"
#include <QtCharts>
using namespace QtCharts;

stats::stats(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stats)
{
    ui->setupUi(this);

    QFile data_file(dataFile);
    if (data_file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&data_file);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList items = line.split(",");
          if(items.takeAt(0).compare("smoked") == 0)
          {
              // day , month , year , time , cig / cann , count , price
              smokeData.append(items);
              /*
              if(items.takeAt(4).compare("cigarrete") == 0)
              {
                  smokeCanData.append(items);
              }
              else{
                  smokeCigData.append(items);
              }
              */
          }
          else{
              skipData.append(items);
          }
       }
       data_file.close();
    }

    QStringList months = QStringList() << tr("January") << tr("February") << tr("March") << tr("April") << tr("May") << tr("June") << tr("July") << tr("August") << tr("September") << tr("October") << tr("November") << tr("December");
    ui->comboMonth_Day->addItems(months);

    QStringList monthsKey = QStringList() << tr("01") << tr("02") << tr("03") << tr("04") << tr("05") << tr("06") << tr("07") << tr("08") << tr("09") << tr("10") << tr("11") << tr("12");

    for (int i = 0 ; i < months.size() ; i++)
    {
        mapMonth.insert(monthsKey.at(i) , months.at(i));
    }
    int firstYear = smokeData.first().at(2).toInt();
    int lastYear = smokeData.at(smokeData.size()-1).at(2).toInt();

    for( int count = firstYear; count <= lastYear ; count++)
    {

        years.append(QString::number(count));
    }

    ui->comboYear_Month->addItems(years);
    ui->comboYear_Day->addItems(years);

    chartYearDisplay();
    chartMonthDisplay("");
    chartDayDisplay("","");
    textStatsDisplay();
}

stats::~stats()
{
    delete ui;
}
void stats::chartMonthDisplay(QString selectedYear = "")
{

    if(selectedYear.compare("") == 0)
    {
        selectedYear = QString::number(QDate::currentDate().year());
        ui->comboYear_Month->setCurrentText(selectedYear);
    }
    QMap<QString,double> costData;
    QMap<QString,double> costCigData;
    QMap<QString,double> costCanData;
    for (int i = 0 ; i < mapMonth.keys().size();i++)
    {
        costData.insert(mapMonth.keys().at(i),0);
        costCigData.insert(mapMonth.keys().at(i),0);
        costCanData.insert(mapMonth.keys().at(i),0);
    }

    for(int i = 0; i < smokeData.size(); i++)
    {
        if(smokeData.at(i).at(2).compare(selectedYear) == 0)
        {
            if(smokeData.at(i).at(4).compare("cigarrete") == 0)
            {
                 costData[smokeData.at(i).at(1)] =  costData[smokeData.at(i).at(1)] + smokeData.at(i).at(6).toDouble() / smokeData.at(i).at(5).toDouble();
                 costCigData[smokeData.at(i).at(1)] =  costCigData[smokeData.at(i).at(1)] +   smokeData.at(i).at(6).toDouble() / smokeData.at(i).at(5).toDouble();

            }
            else {

                 costData[smokeData.at(i).at(1)] =  costData[smokeData.at(i).at(1)] + smokeData.at(i).at(5).toDouble() * smokeData.at(i).at(6).toDouble();
                 costCanData[smokeData.at(i).at(1)] = costCanData[smokeData.at(i).at(1)] +  smokeData.at(i).at(5).toDouble() * smokeData.at(i).at(6).toDouble();
            }

        }
    }

    QBarSet *cost = new QBarSet("Total Monthly Cost");
    for(QString e : costData.keys())
    {
      *cost << costData.value(e);
    }
    QBarSet *costCan = new QBarSet("Cannabis Monthly Cost");
    for(QString e : costCanData.keys())
    {
      *costCan << costCanData.value(e);
    }
    QBarSet *costCig = new QBarSet("Cigarret Monthly Cost");
    for(QString e : costCigData.keys())
    {
      *costCig << costCigData.value(e);
    }

    QBarSeries *seriesCig = new QBarSeries();
    seriesCig->append(costCig);
    QBarSeries *seriesCan = new QBarSeries();
    seriesCan->append(costCan);
    QBarSeries *seriesTotal = new QBarSeries();
    seriesTotal->append(cost);


    QChart *chart = new QChart();
    chart->addSeries(seriesTotal);
    chart->addSeries(seriesCig);
    chart->addSeries(seriesCan);
    chart->setTitle("Stats of month");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(mapMonth.keys());
    chart->addAxis(axisX, Qt::AlignBottom);
    seriesTotal->attachAxis(axisX);
    seriesCan->attachAxis(axisX);
    seriesCig->attachAxis(axisX);


    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Cost($)");
    //axisY->setRange(0,15);
    chart->addAxis(axisY, Qt::AlignLeft);
    seriesTotal->attachAxis(axisY);
    seriesCan->attachAxis(axisY);
    seriesCig->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    ui->chartMonth->setChart(chart);
    ui->chartMonth->setRenderHint(QPainter::Antialiasing);

}
void stats::chartYearDisplay()
{

    QMap<QString,double> costData;
    for (int i = 0 ; i < years.size();i++)
    {
        costData.insert(years.at(i),0);
    }
    QBarSet *cost = new QBarSet("Yearly Cost");
    for(int i = 0; i < smokeData.size(); i++)
    {
         if(smokeData.at(i).at(4).compare("cigarrete") == 0)
            {
                 costData[smokeData.at(i).at(2)] =  costData[smokeData.at(i).at(2)] + smokeData.at(i).at(6).toDouble() / smokeData.at(i).at(5).toDouble();
            }
            else {

                 costData[smokeData.at(i).at(2)] =  costData[smokeData.at(i).at(2)] + smokeData.at(i).at(5).toDouble() * smokeData.at(i).at(6).toDouble();
            }
    }
    for(auto e : costData.keys())
    {
      *cost << costData.value(e);
    }

    QBarSeries *series = new QBarSeries();
    series->append(cost);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Stats of Year");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(years);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Cost($)");
    //axisY->setRange(0,15);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    ui->chartYear->setChart(chart);
    ui->chartYear->setRenderHint(QPainter::Antialiasing);


}
void stats::chartDayDisplay(QString selectedYear = "" , QString selectedMonth = "")
{

    QStringList daysKey = QStringList() <<tr("") << tr("01") << tr("02") << tr("03") << tr("04") << tr("05") << tr("06") << tr("07") << tr("08") << tr("09") << tr("10") << tr("11") << tr("12");
    daysKey = daysKey << tr("13") << tr("14") << tr("15") << tr("16") << tr("17") << tr("18") << tr("19") << tr("20") << tr("21") << tr("22") << tr("23") << tr("24");
    daysKey = daysKey << tr("25") << tr("26") << tr("27") << tr("28") << tr("29") << tr("30") << tr("31");

    QMap<QString,double> costData;
    for (int i = 0 ; i < daysKey.size();i++)
    {
        costData.insert(daysKey.at(i),0);
    }

    if(selectedYear.compare("") == 0)
    {
        selectedYear = QString::number(QDate::currentDate().year());
        ui->comboYear_Day->setCurrentText(selectedYear);

    }

    if(selectedMonth.compare("") == 0)
    {
        selectedMonth = QString::number(QDate::currentDate().month()).rightJustified(2, '0');
        ui->comboMonth_Day->setCurrentText(mapMonth[selectedMonth]);
    }

    for(int i = 0; i < smokeData.size(); i++)
    {
        if(smokeData.at(i).at(2).compare(selectedYear) == 0 && smokeData.at(i).at(1).compare(selectedMonth) == 0)
        {
            if(smokeData.at(i).at(4).compare("cigarrete") == 0)
            {
                 costData[smokeData.at(i).at(0)] =  costData[smokeData.at(i).at(0)] + smokeData.at(i).at(6).toDouble() / smokeData.at(i).at(5).toDouble();
            }
            else {

                 costData[smokeData.at(i).at(0)] =  costData[smokeData.at(i).at(0)] + smokeData.at(i).at(5).toDouble() * smokeData.at(i).at(6).toDouble();
            }

        }
    }


    QLineSeries *series = new QLineSeries();

    for(auto e : costData.keys())
    {
        series->append(e.toInt(), costData[e]);

    }

    //*series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QChart *chart = new QChart();
    //chart->legend()->hide();
    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(daysKey);

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Cost($)");
    //axisY->setRange(0,15);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);


    //chart->createDefaultAxes();
    chart->setTitle("Daily Chart");

    ui->chartDay->setChart(chart);
    ui->chartDay->setRenderHint(QPainter::Antialiasing);

}

void stats::on_btnReset_clicked()
{

    //qDebug() << smokeData;
    chartDayDisplay("","");
    chartMonthDisplay("");
    //data.dat

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Clear Stats", "erase data.dat file ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      qDebug() << "Yes was clicked";
     // QApplication::quit();
      QFile file ("data.dat");
      file.remove();
    } else {
   //   qDebug() << "Yes was *not* clicked";
    }




}

void stats::on_comboYear_Month_currentIndexChanged(int index)
{

   // qDebug() << index << ui->comboYear_Month->itemText(index);
    chartMonthDisplay(ui->comboYear_Month->itemText(index));

}

void stats::on_comboYear_Day_currentIndexChanged(int index)
{
    chartDayDisplay(ui->comboYear_Day->itemText(index) ,mapMonth.key( ui->comboMonth_Day->itemText(index) ));
}

void stats::on_comboMonth_Day_currentIndexChanged(int index)
{
    chartDayDisplay(ui->comboYear_Day->itemText(index) , mapMonth.key( ui->comboMonth_Day->itemText(index) ));
}
void stats::textStatsDisplay()
{
    QString currentYear = QString::number(QDate::currentDate().year());
    QString currentMonth = QString::number(QDate::currentDate().month()).rightJustified(2, '0');
    QString lastYear = QString::number(QDate::currentDate().year() - 1);
    QString lastMonth;
    int IsDec = 0;
    if ( QDate::currentDate().month() == 1)
    {
        lastMonth = QString::number(12);
        IsDec = 1;
    }else {
        lastMonth = QString::number(QDate::currentDate().month() - 1).rightJustified(2, '0');
    }

    double thisYearCost = 0;
    double lastYearCost = 0;
    double thisMonthCost = 0;
    double lastMonthCost = 0;

    for(int i = 0; i < smokeData.size(); i++)
    {
        if(smokeData.at(i).at(2).compare(currentYear) == 0)
        {
            if(smokeData.at(i).at(1).compare(currentMonth) == 0)
            {
                if(smokeData.at(i).at(4).compare("cigarrete") == 0)
                {
                     thisMonthCost = thisMonthCost  + smokeData.at(i).at(6).toDouble() / smokeData.at(i).at(5).toDouble();
                }
                else {

                     thisMonthCost =  thisMonthCost + smokeData.at(i).at(5).toDouble() * smokeData.at(i).at(6).toDouble();
                }
            }

            if(IsDec == 0)
            {
                if(smokeData.at(i).at(1).compare(lastMonth) == 0)
                {
                    if(smokeData.at(i).at(4).compare("cigarrete") == 0)
                    {
                         lastMonthCost = lastMonthCost  + smokeData.at(i).at(6).toDouble() / smokeData.at(i).at(5).toDouble();
                    }
                    else {

                         lastMonthCost =  lastMonthCost + smokeData.at(i).at(5).toDouble() * smokeData.at(i).at(6).toDouble();
                    }
                }
            }


            if(smokeData.at(i).at(4).compare("cigarrete") == 0)
            {
                 thisYearCost = thisYearCost  + smokeData.at(i).at(6).toDouble() / smokeData.at(i).at(5).toDouble();
            }
            else {

                 thisYearCost =  thisYearCost + smokeData.at(i).at(5).toDouble() * smokeData.at(i).at(6).toDouble();
            }

        }else {
            if(smokeData.at(i).at(2).compare(lastYear) == 0)
            {
                if(IsDec == 1)
                {
                    if(smokeData.at(i).at(1).compare(lastMonth) == 0)
                    {
                        if(smokeData.at(i).at(4).compare("cigarrete") == 0)
                        {
                             lastMonthCost = lastMonthCost  + smokeData.at(i).at(6).toDouble() / smokeData.at(i).at(5).toDouble();
                        }
                        else {

                             lastMonthCost =  lastMonthCost + smokeData.at(i).at(5).toDouble() * smokeData.at(i).at(6).toDouble();
                        }
                    }
                }

                if(smokeData.at(i).at(4).compare("cigarrete") == 0)
                {
                     lastYearCost = lastYearCost  + smokeData.at(i).at(6).toDouble() / smokeData.at(i).at(5).toDouble();
                }
                else {

                     lastYearCost =  lastYearCost + smokeData.at(i).at(5).toDouble() * smokeData.at(i).at(6).toDouble();
                }

            }
        }
    }
    ui->editCostThisYear->setText(QString::number(thisYearCost));
    ui->editCostLastYear->setText(QString::number(lastYearCost));
    ui->editCostThisMonth->setText(QString::number(thisMonthCost));
    ui->editCostLastMonth->setText(QString::number(lastMonthCost));
    ui->editYearSaving->setText(QString::number(lastYearCost - thisYearCost));
    ui->editMonthSaving->setText(QString::number(lastMonthCost - thisMonthCost));

}

