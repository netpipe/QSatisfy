#include "settings.h"
#include "ui_settings.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
settings::settings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);





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
    ui->editPackCost->setText(cigarettesCost);
    ui->editPackCount->setText(cigarettesCount);
    ui->editCostGram->setText(cannabisCost);
    ui->editGramOnce->setText(cannabisCount);
    ui->editTimerPeriod->setText(timerPeriod);
    ui->editBackgroudImage->setText(backgroundImage);
    if((soundEnable.compare("1") == 0) == (soundEnable == "1"))
    {
        ui->radioSoundY->setChecked(true);
        ui->radioSoundN->setChecked(false);
    }
    else {
        ui->radioSoundY->setChecked(false);
        ui->radioSoundN->setChecked(true);
    }
    if((messageEnable.compare("1") == 0) == (messageEnable == "1"))
    {
        ui->radioMessageY->setChecked(true);
        ui->radioMessageN->setChecked(false);
    }
    else {
        ui->radioMessageY->setChecked(false);
        ui->radioMessageN->setChecked(true);
    }



       /* */
}

settings::~settings()
{
    delete ui;
}

void settings::on_btnCancel_clicked()
{
 this->close();
}

void settings::on_btnSave_clicked()
{
    QFile config_file(configFile);
    if (config_file.open(QFile::WriteOnly | QFile::Text))
    {
       QTextStream out(&config_file);
       QMap<QString, QString> config;

       config["cigarettesCost"] = ui->editPackCost->text();
       config["cigarettesCount"]  = ui->editPackCount->text();
       config["timer"] = ui->editTimerPeriod->text();
       config["cannabisCost"] = ui->editCostGram->text();
       config["cannabisCount"] = ui->editGramOnce->text();
       config["backgroundImage"] = ui->editBackgroudImage->text();

       if(ui->radioSoundY->isChecked())
       {
          config["sound"] = "1";
       }
       else {
           config["sound"] = "0";
       }

       if(ui->radioMessageY->isChecked())
       {
          config["message"] = "1";
       }
       else {
           config["message"] = "0";
       }

       QMap<QString, QString>::iterator it;

       for (it = config.begin(); it != config.end(); ++it) {
          // output += QString("%1 : %2").arg(it.key()).arg(it.value());
            out << it.key()+","+it.value() << endl;
       }
       config_file.close();
       QMessageBox msgBox;
       msgBox.setText("Saved!");
       msgBox.exec();
       this->hide();

    }
}

void settings::on_btnSelectImage_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/",tr("Images (*.png *.jpg)"));
    ui->editBackgroudImage->setText(fileName);
}
