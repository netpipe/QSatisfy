#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stats.h"
#include "settings.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QSound>
QString mediadir = "./media/"; //change mediadir2 in main.cpp aswell if needed


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->radioCigarrete->setChecked(true);

    QFile config_file(configFile);
    if (config_file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&config_file);
       QMap<QString, QString> config;
       while (!in.atEnd())
       {
          QString line = in.readLine();
          config.insert(line.split(",").takeAt(0) , line.split(",").takeAt(1));

       }
       config_file.close();

       cigarettesCost = config["cigarettesCost"];
       cigarettesCount = config["cigarettesCount"];
       timerPeriod = config["timer"];
       cannabisCost = config["cannabisCost"];
       cannabisCount = config["cannabisCount"];
       messageEnable = config["message"];
       soundEnable = config["sound"];
       backgroundImage = config["backgroundImage"];


    }
    ui->editCostPack->setText(cigarettesCost);
    ui->editPackCount->setText(cigarettesCount);
    ui->editCostGram->setText(cannabisCost);
    ui->editGramCount->setText(cannabisCount);

   // QPixmap pix("smoking.png");
      //  ui->label->setPixmap(pix);

      //  QLabel lblImage;
//      printf("bg image is %s",backgroundImage);



        if ( backgroundImage != "")
        {         ui->label->setPixmap( backgroundImage ); }
        else { ui->label->setPixmap( QPixmap( mediadir + "smoking.png" ) );  };

        ui->label->setScaledContents( true );
        ui->label->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    // trying set background image...
   //setStyleSheet("MainWindow {background-image:url(:" + backgroundImage + ")}");
   // this->centralWidget()->setStyleSheet( "background-image:url(\"F:\final my resume\2016-08-10.jpg\"); background-position: center;" );

   // QPixmap bkgnd(backgroundImage);
   // bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
 //   QPalette palette;
//    palette.setBrush(QPalette::Window, bkgnd);
    //this->centralWidget()->setPalette(palette);
    //this->centralWidget()->setAutoFillBackground(true);
    /**/

    connect(ui->btnViewStats, SIGNAL(clicked()), this, SLOT(openStatsWindow()));
    connect(ui->btnViewSetting, SIGNAL(clicked()), this, SLOT(openSettingsWindow()));


    //ui->btnSmoked->setStyleSheet("background-color: red; border-width: 0px; margin: 4; border-radius: 5px; border-style: outset");
    //ui->btnSkipped->setStyleSheet("background-color: green; border-width: 0px; margin: 4; border-radius: 5px; border-style: outset");



    QPixmap oPixmap(32,32);
    oPixmap.load ( mediadir + "smoking.png");

    QIcon oIcon( oPixmap );

    trayIcon = new QSystemTrayIcon(oIcon);

    QAction *quit_action = new QAction( "Exit", trayIcon );
    connect( quit_action, SIGNAL(triggered()), this, SLOT(on_exit()) );

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction( quit_action );

    trayIcon->setContextMenu( trayIconMenu);
    trayIcon->setVisible(true);
    //trayIcon->showMessage("Test Message", "Text", QSystemTrayIcon::Information, 1000);
    //trayIcon->show();


    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    ui->timeCountProgress->setRange(0, int(timerPeriod.toFloat() * 60));
    ui->timeCountProgress->setValue(0);

    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(updateCheck()));
    //timer->start(int(timerPeriod.toFloat() * 60) * 1000); // int(timerPeriod) * 60 * 1000

    QTimer *timerClock = new QTimer(this);
    connect(timerClock,SIGNAL(timeout()), this, SLOT(showTime()));
    timerClock->start(1000);

    showTime();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateCheck()
{

    // this->setFocus(); this does not work :-(

    //ui->timeCountProgress->setValue(0);

    this->show();
    //this->setWindowFlags((windowFlags() & Qt::WindowStaysOnTopHint));
    this->activateWindow();
    this->raise();
   // this->setWindowState(Qt::WindowActive);
    if( soundEnable.compare("1") == 0)
    {

        QSound::play( mediadir + "phone.wav");
    }
    if(messageEnable.compare("1") == 0)
    {
        showMessage();
    }

}
void MainWindow::showTime()
{
    QTime time = QTime::currentTime();
   QString text = time.toString("hh:mm");
   if ((time.second() % 2) == 0)
       text[2] = ' ';
   ui->timeLcd->display(text);

   //qDebug()<<  ui->timeCountProgress->value();
   ui->timeCountProgress->setValue( ui->timeCountProgress->value() + 1);

   if(ui->timeCountProgress->value() == ui->timeCountProgress->maximum())
   {
       updateCheck();
       ui->timeCountProgress->setValue(0);
   }

}

void MainWindow::on_btnSmoked_clicked()
{

    QString currentDate = QDate::currentDate().toString("dd,MM,yyyy");
    QString type = "";
    QString count = "";
    QString cost = "";
    if(ui->radioCigarrete->isChecked())
    {
         type = "cigarrete";
         count = ui->editPackCount->text();
         cost = ui->editCostPack->text();
    }
    else {
        type = "cannabis";
        count = ui->editGramCount->text();
        cost = ui->editCostGram->text();
    }

    QFile file(dataFile);
    file.open(QFile::Append | QFile::Text);
    QTextStream out(&file);   // we will serialize the data into the file
    out << QString("smoked") + "," + currentDate + "," + QTime::currentTime().toString()+","+type+","+count+","+cost<< endl;
    file.close();
     this->hide();
    ui->timeCountProgress->setValue(0);
    QMessageBox msgBox;
    msgBox.setText("Smoked");
    msgBox.exec();

}

void MainWindow::on_btnSkipped_clicked()
{

    QString currentDate = QDate::currentDate().toString("dd,MM,yyyy");
    QString type = "";
    QString count = "";
    QString cost = "";
    if(ui->radioCigarrete->isChecked())
    {
         type = "cigarrete";
         count = ui->editPackCount->text();
         cost = ui->editCostPack->text();
    }
    else {
        type = "cannabis";
        count = ui->editGramCount->text();
        cost = ui->editCostGram->text();
    }

    QFile file(dataFile);
    file.open(QFile::Append | QFile::Text);
    QTextStream out(&file);   // we will serialize the data into the file
    out << QString("skipped")  + "," + currentDate + "," + QTime::currentTime().toString()+","+type+","+count+","+cost << endl;
    file.close();
    this->hide();
    ui->timeCountProgress->setValue(0);
    QMessageBox msgBox;
    msgBox.setText("Skipped.");
    msgBox.exec();

}

//void MainWindow::on_btnHide_clicked()
//{
   // this->hide();
//}
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
     case QSystemTrayIcon::Trigger:
         this->show();
         break;
     case QSystemTrayIcon::DoubleClick:
         this->show();
         break;
     case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;

     default:
         ;
    }
}
void MainWindow::showMessage()
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
    trayIcon->showMessage(tr("QSatisfy"), tr("Will you smoke now..."), icon, 100);
}
void MainWindow::on_exit()
{
    this->close();
    QApplication::quit();
}
void MainWindow::openStatsWindow()
{
    statsWindow = new stats();
    statsWindow->show();
    this->hide();
   // deleteLater();
}
void MainWindow::openSettingsWindow()
{
    this->hide();
    settingsWindow = new settings();
    settingsWindow->show();


}
void MainWindow::showEvent( QShowEvent* event ) {
    QWidget::showEvent( event );


    QFile config_file(configFile);
    if (config_file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&config_file);
       QMap<QString, QString> config;
       while (!in.atEnd())
       {
          QString line = in.readLine();
          config.insert(line.split(",").takeAt(0) , line.split(",").takeAt(1));

       }
       config_file.close();

       cigarettesCost = config["cigarettesCost"];
       cigarettesCount = config["cigarettesCount"];

       if(timerPeriod.compare(config["timer"]) != 0)
       {
           ui->timeCountProgress->setRange(0, int(config["timer"].toFloat() * 60));
           ui->timeCountProgress->setValue(0);

       }
       timerPeriod = config["timer"];
       cannabisCost = config["cannabisCost"];
       cannabisCount = config["cannabisCount"];
       messageEnable = config["message"];
       soundEnable = config["sound"];

       if(backgroundImage.compare(config["backgroundImage"]) != 0)
       {
           QPixmap bkgnd(config["backgroundImage"]);
           bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
           QPalette palette;
           palette.setBrush(QPalette::Window, bkgnd);
           this->centralWidget()->setPalette(palette);
           this->centralWidget()->setAutoFillBackground(true);

       }

       backgroundImage = config["backgroundImage"];

    }
    ui->editCostPack->setText(cigarettesCost);
    ui->editPackCount->setText(cigarettesCount);
    ui->editCostGram->setText(cannabisCost);
    ui->editGramCount->setText(cannabisCount);



}

void MainWindow::on_actionExit_triggered()
{
    this->close();
    QApplication::quit();
}

void MainWindow::on_actionSetting_triggered()
{
    this->hide(); // needs this for now
    settingsWindow = new settings();
    settingsWindow->show();
}

void MainWindow::on_actionstats_triggered()
{
    statsWindow = new stats();
    statsWindow->show();
    this->hide();
}
