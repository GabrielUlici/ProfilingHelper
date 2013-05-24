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

#ifndef CREATEINDEXDIALOG_H
#define CREATEINDEXDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QtSql>

namespace Ui {
    class CreateIndexDialog;
}

class CreateIndexDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateIndexDialog(QWidget *parent = 0);
    ~CreateIndexDialog();
    QSqlDatabase db;
    void setTableName(QString tableName);
signals:
    void emitLog(QString, QString);
private:
    Ui::CreateIndexDialog *ui;
    QString getSQL();
    QString indexTable;
private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // CREATEINDEXDIALOG_H
