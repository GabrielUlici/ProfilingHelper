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

#ifndef ADVANCEDSEARCHDIALOG_H
#define ADVANCEDSEARCHDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QtSql>

namespace Ui {
    class AdvancedSearchDialog;
}

class AdvancedSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdvancedSearchDialog(QWidget *parent = 0, QSqlDatabase dbToUse = QSqlDatabase(), QString tableName = "");
    ~AdvancedSearchDialog();
    QSqlDatabase db;
    QString str_tableName;
    QString searchQuery;
private:
    Ui::AdvancedSearchDialog *ui;
    QString generateSQL();
private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_cmdAddtoSearch_clicked();
};

#endif // ADVANCEDSEARCHDIALOG_H
