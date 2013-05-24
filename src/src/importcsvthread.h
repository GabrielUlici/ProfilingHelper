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

#ifndef IMPORTCSVTHREAD_H
#define IMPORTCSVTHREAD_H

#include <QThread>

class ImportCSVThread : public QThread
{
    Q_OBJECT
public:
    explicit ImportCSVThread(QObject *parent = 0);

signals:

public slots:

};

#endif // IMPORTCSVTHREAD_H
