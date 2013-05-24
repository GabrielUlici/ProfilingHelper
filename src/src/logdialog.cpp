/**********************************************************************
**              Author: Gabriel Ulici
**
** Copyright (C) 2011 Night Ideas Inc & LabRoCoCo
**             All rights reserved.
**
**
**               Contact:
**
** Gabriel Ulici: ulicigabriel@gmail.com
**                  or
** LabRoCoCo: labrococo.dis.uniroma1.it
**
************************************************************************/

#include "logdialog.h"
#include "ui_logdialog.h"

LogDialog::LogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogDialog)
{
    ui->setupUi(this);
    ui->lstApp->setVisible(false);
}

LogDialog::~LogDialog()
{
    emit closed(false);
    delete ui;
}

void LogDialog::closeEvent( QCloseEvent * )
{
        emit closed(false);
}

void LogDialog::refreshLog(QString section, QString newLog) {
    if (section == "user") {
        userLogs.insert(userLogs.count(), newLog);
        ui->lstUser->insertItem(0, newLog);
    } else {
        appLogs.insert(appLogs.count(), newLog);
        ui->lstApp->insertItem(0, newLog);
    }

}
