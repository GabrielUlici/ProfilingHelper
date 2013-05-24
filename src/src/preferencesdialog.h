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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QStyleFactory>
#include <QSettings>

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();
    QFont queryEditorFont;
    int queryEditorFontSize;
    QFont queryBrowserFont;
    int queryBrowserFontSize;
    QFont databaseExplorerFont;
    int databaseExplorerFontSize;

private:
    Ui::PreferencesDialog *ui;
        QSettings *sett;

private slots:
    void on_buttonBox_accepted();
};

#endif // PREFERENCESDIALOG_H
