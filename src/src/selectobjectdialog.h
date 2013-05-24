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

#ifndef SELECTOBJECTDIALOG_H
#define SELECTOBJECTDIALOG_H

#include <QDialog>

namespace Ui {
    class SelectObjectDialog;
}

class SelectObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectObjectDialog(QWidget *parent = 0, QString title = tr("Select Object"), QString label = tr("Select Object"));
    ~SelectObjectDialog();
    void setItems(QStringList items);    
    QString returnValue();
private:
    Ui::SelectObjectDialog *ui;
};

#endif // SELECTOBJECTDIALOG_H
