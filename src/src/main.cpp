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

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("buHuHu Inc.");
    a.setApplicationName("Profiling Helper");
    MainWindow w;
    QObject::connect(&w ,SIGNAL(statusMessage(QString)),
                     w.statusBar(), SLOT(showMessage(QString)));
    w.show();
    return a.exec();
}
