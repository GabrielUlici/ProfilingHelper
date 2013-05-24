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

#include "importcsvdialog.h"
#include "ui_importcsvdialog.h"

ImportCSVDialog::ImportCSVDialog(QWidget *parent, QSqlDatabase DbToUse, QString fileName) :
        QDialog(parent),
        ui(new Ui::ImportCSVDialog)
{
    ui->setupUi(this);
    db = DbToUse;
    str_fileName = fileName;
    ui->lblFileName->setText(tr("File Name: %1").arg(fileName));
    ui->separatorComboBox->addItem(" ");
    ui->separatorComboBox->addItem(",");
    ui->delimiterComboBox->addItem("");
    ui->delimiterComboBox->addItem("''");
    ui->delimiterComboBox->addItem("'");
    connect (ui->chkFirstRow, SIGNAL(toggled(bool)), this, SLOT(parseFile()));
    parseFile();
}

ImportCSVDialog::~ImportCSVDialog()
{
    delete ui;
}

void ImportCSVDialog::parseFile()
{
    QFile fil(str_fileName);
    if (fil.open(QIODevice::ReadOnly)) {
        ui->tblPreview->clear();
        QTextStream in(&fil);
        QString csv = in.readAll();
        QStringList lines = csv.split("\n");
        int limit = ui->spinNumRows->value();
        if (limit == 0 || limit > lines.count())
            limit = lines.count();
        if (ui->chkFirstRow->isChecked())
            limit = limit - 1;
        ui->tblPreview->setRowCount(limit - 1);
        QString headerLine = lines.at(0)        ;
        QStringList fList = headerLine.split(ui->separatorComboBox->currentText());
        ui->tblPreview->setColumnCount(fList.count());
        for (int f = 0; f < fList.count(); ++f) {
            QString headerTxt = "";
            if (ui->chkFirstRow->isChecked()) {
                headerTxt = fList.at(f);
                headerTxt = headerTxt.remove(ui->delimiterComboBox->currentText(), Qt::CaseInsensitive);
            } else {
                headerTxt = QString::number(f + 1);
            }
            QTableWidgetItem *it = new QTableWidgetItem(headerTxt, 1);
            ui->tblPreview->setHorizontalHeaderItem(f, it);
        }
        int start = 0;
        if (ui->chkFirstRow->isChecked())
            start = 1;
        for (int l = start; l < limit ; ++l) {
            QString thisLine = lines.at(l);
            QStringList fields = thisLine.split(ui->separatorComboBox->currentText());
            int row = l;
            if (l == 0) {
                 if (!ui->chkFirstRow->isChecked()) {
                    for (int f = 0; f < fields.count(); ++f) {
                        QString cellTxt = "";
                        cellTxt = fields.at(f);
                        cellTxt = cellTxt.remove(ui->delimiterComboBox->currentText(), Qt::CaseInsensitive);
                        QTableWidgetItem *it = new QTableWidgetItem(cellTxt, 1);
                        ui->tblPreview->setItem(row, f, it);
                    }
                }
            } else {
                if (ui->chkFirstRow->isChecked())
                    row = l - 1;
                for (int f = 0; f < fields.count(); ++f) {
                    QString cellTxt = "";
                    cellTxt = fields.at(f);
                    cellTxt = cellTxt.remove(ui->delimiterComboBox->currentText(), Qt::CaseInsensitive);
                    QTableWidgetItem *it = new QTableWidgetItem(cellTxt, 1);
                    ui->tblPreview->setItem(row, f, it);
                }
            }
        }
        fil.close();
    }
}

bool ImportCSVDialog::generateSQL() {
    QFile fil(str_fileName);
    QString sql = "";
    if (fil.open(QIODevice::ReadOnly)) {
        QTextStream in(&fil);
        QString allLines = in.readAll();
        QStringList lines = allLines.split("\n");
        sql += "CREATE TABLE \"main\".\"" + ui->txtTableName->text() + "\"(";
        QString headerLine = lines.at(0);
        QStringList headerItems = headerLine.split(ui->separatorComboBox->currentText());
        int headerCount =  headerItems.count();
        for (int h = 0; h < headerCount; ++h) {
            QString headerValue = QString::number(h + 1);
            headerValue = "\"" + headerValue + "\"" + ", ";
            if (ui->chkFirstRow->isChecked()) {
                headerValue = headerItems.at(h) + ", ";
            }
            sql += headerValue;
        }
        sql = sql.remove(sql.size() - 3, 2);
        sql += ");\n";
        QString createTableSQL = sql;
        QSqlQuery qu(createTableSQL, db);
        if (!qu.lastError().isValid()) {
            int startPoint = 0;
            if (ui->chkFirstRow->isChecked())
                startPoint = 1;
            for (int i = startPoint; i < lines.count(); ++i) {
                QString thisLine = lines.at(i);
                QStringList lineItems = thisLine.split(ui->separatorComboBox->currentText());
                QString lineSQL = "INSERT INTO " + ui->txtTableName->text() + " VALUES (";
                for (int c = 0; c < lineItems.count(); ++c) {
                    QString val = lineItems.at(c);
                    val = val.remove(ui->delimiterComboBox->currentText());
                    val.replace("\\N", "\\n");
                    val.replace("'", "''");
                    lineSQL += "'" + val + "', ";
                }
                lineSQL = lineSQL.remove(lineSQL.size() - 2, 2);
                lineSQL += ");";
                sql += lineSQL;
                db.exec(lineSQL);
            }
            fil.close();
            return true;
        } else {
            QMessageBox::critical(this, tr("Create Table Error"), qu.lastError().text());
            return false;
            fil.close();
        }
    } else {
        return false;
    }
}

void ImportCSVDialog::on_buttonBox_accepted()
{
    if (generateSQL()) {
        this->accept();
    } else {
        return;
    }
}

void ImportCSVDialog::on_buttonBox_rejected()
{
    this->reject();
}

void ImportCSVDialog::on_actionRefreshPreview_clicked()
{
    parseFile();
}
