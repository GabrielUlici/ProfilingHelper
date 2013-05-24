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

#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>

namespace Ui {
    class LogDialog;
}

class LogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = 0);
    ~LogDialog();
    QStringList userLogs;
    QStringList appLogs;
private:
    Ui::LogDialog *ui;
signals:
    void closed(bool);
public slots:
    virtual void closeEvent( QCloseEvent * );
    void refreshLog(QString section, QString newLog);
};

#endif // LOGDIALOG_H
