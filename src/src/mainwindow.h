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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtSql>
#include "database.h"
#include "sqlmodels.h"
#include <QSettings>
#include "createtabledialog.h"
#include "aboutdialog.h"
#include "logdialog.h"
#include "highlighter.h"
#include "selectobjectdialog.h"
#include "createindexdialog.h"
#include "preferencesdialog.h"
#include "advancedsearchdialog.h"
#include "importcsvdialog.h"
#include "createviewdialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
        void dragEnterEvent(QDragEnterEvent *event);
        void dropEvent(QDropEvent *event);
        void changeEvent(QEvent *);
    protected slots:

signals:
    void statusMessage(const QString &message);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QActionGroup *tableActions;
    QActionGroup *indexActions;
    QActionGroup *databaseActions;
    QSqlDatabase db;
    SqlTableModel *model;
    LogDialog *myLog;
    Highlighter *highlighter;
    QStringList tableList;
    QStringList indexList;
    QStringList viewList;
    QTranslator m_translator;
    QTranslator m_translatorQt;
    QString m_currLang;
    QString m_langPath;

private slots:
    void on_cmdExecuteQuery_clicked();
    void on_cmdSaveChanges_clicked();
    void on_cmdDeleteRecord_clicked();
    void on_cmdNewRecord_clicked();
    void showTable(QString tableName);
    void showTable();
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void helpPA();
    void changelog();
    void reportBug();
    void knownbugs();
    void sqlhelp();
    void moreInfo();
    void openExempleWindow();
    void about();
    void closeFile();
    void openRecentFile();
    void newTable();
    void editTable();
    void dropTable();
    void dropIndex();
    void createIndex();
    void showPreferences();
    void preferencesChanged();
    void exportDatabaseAsSQL();
    void importDatabaseFromSQL();
    void exportTableToCSV();
    void importTableFromCSV();
    void openImportWindow();
    void compactDb();
    void searchAll();
    void advancedSearch();
    QString getTableCreateSQL(QString tableName);
    QString getTableInsertSQL(QString tableName);
    void reloadDbList();
    void newView();
    void modifyView();
    void deleteView();
    void on_loadQuery_clicked();
    void on_saveQuery_clicked();
    void on_clearButton_clicked();
    void on_searchButton_clicked();
    void on_actionoutBallPercept_clicked();
    void on_actionoutBallInImage_clicked();
    void on_actionoutGoalMineSeen_clicked();
    void on_actionoutGoalMineLeftPost_clicked();
    void on_actionoutGoalOpponentSeen_clicked();
    void on_actionoutGoalOpponentLeftPost_clicked();
    void on_actioninGoalMineSeen_clicked();
    void on_actioninGoalMineLeftPost_clicked();
    void on_actioninGoalOpponentSeen_clicked();
    void on_actioninCoordinateBallField_clicked();
    void on_actioninCoordinateBallImage_clicked();
    void on_actioninGoalOpponentLeftPost_clicked();
    void on_actionModuleDuration_clicked();
    void on_actionPropLockUnlock_clicked();
    void on_actionPropReadWritten_clicked();
    void on_actionFrequencyModule_clicked();
    void on_actionAll_clicked();
    void actionLoadanExample();

private:
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    QString curFile;
    void updateRecentFileActions();
    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QString getTable();
    QString getTableNView();
    QString getIndex();
    QString getView();
    void doSearch(QString searchQuery);
};

#endif // MAINWINDOW_H


