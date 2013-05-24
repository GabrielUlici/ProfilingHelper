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

#include "selectobjectdialog.h"
#include "ui_selectobjectdialog.h"

SelectObjectDialog::SelectObjectDialog(QWidget *parent, QString title, QString label) :
    QDialog(parent),
    ui(new Ui::SelectObjectDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->label->setText(label);
}

SelectObjectDialog::~SelectObjectDialog()
{
    delete ui;
}

void SelectObjectDialog::setItems(QStringList items)  {
    ui->comboBox->insertItems(0, items);
}

QString SelectObjectDialog::returnValue() {
    return ui->comboBox->currentText();
}
