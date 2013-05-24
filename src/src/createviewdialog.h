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

#ifndef CREATEVIEWDIALOG_H
#define CREATEVIEWDIALOG_H

#include <QtGui>
#include <QDialog>
#include <QtSql>
#include "highlighter.h"

namespace Ui {
    class CreateViewDialog;
}

class CreateViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateViewDialog(QWidget *parent = 0);
    ~CreateViewDialog();
    QSqlDatabase db;
    void setViewName(QString viewName);
private:
    Ui::CreateViewDialog *ui;
    QString getSQL();
    Highlighter *highlighter;
    bool isModify;

private slots:
    void on_cmdCancel_clicked();
    void onSave();
};

#endif // CREATEVIEWDIALOG_H
