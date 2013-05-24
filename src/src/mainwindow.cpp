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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set window central
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth, width;
    int screenHeight, height;
    int x, y;
    QSize windowSize;
    screenWidth = desktop->width(); // get width of screen
    screenHeight = desktop->height(); // get height of screen
    windowSize = size(); // size of our application window
    width = windowSize.width();
    height = windowSize.height();
    // little computations
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    y -= 50;
    // move window to desired coordinates
    move ( x, y );
    //set window central

    setAcceptDrops(true);
   // ui->tblData->addAction(insertRowAction);
    ui->tblData->addAction(ui->insertRowAction);
    highlighter =  new Highlighter(ui->txtQuery->document());
    myLog = new LogDialog(this);
    connect (ui->actionOpen_Database, SIGNAL(triggered()), SLOT(open()));
    connect (ui->actionSave_Database, SIGNAL(triggered()), SLOT(save()));
    connect (ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    connect (ui->actionAbout_Profiling_Tool, SIGNAL(triggered()), SLOT(helpPA()));
    connect (ui->actionAbout_Profiling_File, SIGNAL(triggered()), SLOT(moreInfo()));
    connect (ui->actionReport_a_bug, SIGNAL(triggered()), SLOT(reportBug()));
    connect (ui->actionExample, SIGNAL(triggered()), SLOT(openExempleWindow()));
    connect (ui->actionCreate_Table, SIGNAL(triggered()), SLOT(newTable()));
    connect (ui->actionModify_Table, SIGNAL(triggered()), SLOT(editTable()));
    connect (ui->actionDelete_Table, SIGNAL(triggered()), SLOT(dropTable()));
    connect (ui->actionDelete_Index, SIGNAL(triggered()), SLOT(dropIndex()));
    connect (ui->actionCreate_Index, SIGNAL(triggered()), SLOT(createIndex()));
   // (ui->actionPreferences, SIGNAL(triggered()), SLOT(showPreferences())); //for the moment this is not working connect
    connect (ui->actionDatabse_As_SQL_File, SIGNAL(triggered()), SLOT(exportDatabaseAsSQL()));
    connect (ui->actionDatabase_From_SQL_File, SIGNAL(triggered()), SLOT(importDatabaseFromSQL()));
    connect (ui->actionTable_as_CSV_File, SIGNAL(triggered()), SLOT(exportTableToCSV()));
    connect (ui->actionTable_From_CSV_File, SIGNAL(triggered()), SLOT(openImportWindow()));
   // connect (ui->actionCompact_Database, SIGNAL(triggered()), SLOT(compactDb()));
    connect (ui->cmdSearch, SIGNAL(clicked()), SLOT(searchAll()));
    connect (ui->cmdAdvancedSearch, SIGNAL(clicked()), SLOT(advancedSearch()));
    connect (ui->cboTables, SIGNAL(currentIndexChanged(QString)), SLOT(showTable(QString)));
    connect (ui->cmdRefresh, SIGNAL(clicked()), SLOT(showTable()));
    connect (ui->actionSQL_Log, SIGNAL(toggled(bool)), myLog, SLOT(setVisible(bool)));
    connect (ui->actionRefresh, SIGNAL(triggered()), SLOT(reloadDbList()));
    connect (myLog, SIGNAL(closed(bool)), ui->actionSQL_Log, SLOT(setChecked(bool)));
    connect (ui->actionCreate_View, SIGNAL(triggered()), SLOT(newView()));
    connect (ui->actionModify_View, SIGNAL(triggered()), SLOT(modifyView()));
    connect (ui->actionDelete_View, SIGNAL(triggered()), SLOT(deleteView()));
    connect (ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect (ui->actionChange_Log, SIGNAL(triggered()), SLOT(changelog()));
    connect (ui->actionKnown_Bugs, SIGNAL(triggered()), SLOT(knownbugs()));
    connect (ui->actionQuery_Help, SIGNAL(triggered()), SLOT(sqlhelp()));
    tableActions = new QActionGroup(this);
    indexActions = new QActionGroup(this);
    databaseActions = new QActionGroup(this);
    indexActions->addAction(ui->actionCreate_Index);
    indexActions->addAction(ui->actionDelete_Index);
    tableActions->addAction(ui->actionCreate_Table);
    tableActions->addAction(ui->actionModify_Table);
    tableActions->addAction(ui->actionDelete_Table);
    tableActions->addAction(ui->actionTable_as_CSV_File);
    tableActions->addAction(ui->actionCreate_View);
    tableActions->addAction(ui->actionModify_View);
    tableActions->addAction(ui->actionDelete_View);
    tableActions->addAction(ui->actionTable_From_CSV_File);
    databaseActions->addAction(ui->actionSave_Database);
    databaseActions->addAction(ui->actionRevert_Database);
    databaseActions->addAction(ui->actionDatabse_As_SQL_File);
    databaseActions->addAction(ui->actionClose_Database);
    databaseActions->addAction(ui->actionCompact_Database);
    databaseActions->addAction(ui->actionRefresh);
    ui->actionSave_Database->setVisible(false);
    ui->actionRevert_Database->setVisible(false);
    model = new SqlTableModel(this, db);
    closeFile();
    connect (ui->actionClose_Database, SIGNAL(triggered()), SLOT(closeFile()));
    connect (ui->actionNew_Database, SIGNAL(triggered()), SLOT(newFile()));
    ui->menuFile->removeAction(ui->actionExit);
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect (recentFileActs[i], SIGNAL(triggered()), SLOT(openRecentFile()));
        ui->menuFile->addAction(recentFileActs[i]);
    }
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(ui->actionExit);
    ui->resultTab->setEnabled(false);
    ui->actionCompact_Database->setEnabled(false);
    updateRecentFileActions();
    QSettings sett(qApp->organizationName(), qApp->applicationName()) ;
    preferencesChanged();
}

MainWindow::~MainWindow()
{
    QSettings sett(qApp->organizationName(), qApp->applicationName());
    delete ui;
}

void MainWindow::newFile() {
    saveAs();
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
        tr("SQLitedb Files (*.sqlitedb);;SQLite Files (*.sqlite)"));
    if (!fileName.isEmpty())
        loadFile(fileName);
}

bool MainWindow::save() {
    return true;
}

bool MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("New Database"), "",
        tr("SQLitedb Files (*.sqlitedb);;SQLite Files (*.sqlite)"));
    if (!fileName.isEmpty())
        loadFile(fileName);
    return !fileName.isEmpty();
}

void MainWindow::helpPA()
{
        QDesktopServices::openUrl(QUrl ("http://labrococo.dis.uniroma1.it/dokuwiki/doku.php?id=tut:profiling"));
}

void MainWindow::changelog()
{
        QDesktopServices::openUrl(QUrl ("http://labrococo.dis.uniroma1.it/dokuwiki/doku.php?id=tut:profiling#change_log"));
}

void MainWindow::reportBug()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Report Bug");
    msgBox.setText("If you have problems with the application\n\n "
                   "send and email to ulicigabriel@gmail.com.\n");
    QAbstractButton *myYesButton = msgBox.addButton(trUtf8("Send"), QMessageBox::YesRole);
    QAbstractButton *myNoButton = msgBox.addButton(trUtf8("Cancel"), QMessageBox::NoRole);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
    if(msgBox.clickedButton() == myYesButton)
    {
        QDesktopServices::openUrl(QUrl ("mailto:ulicigabriel@gmail.com?Subject=Profiling%20Helper%20Bug"));
    }
    if(msgBox.clickedButton() == myNoButton)
    {
    }
}

void MainWindow::sqlhelp()
{
        QDesktopServices::openUrl(QUrl ("http://www.w3schools.com/sql/"));
}

void MainWindow::knownbugs()
{
        QDesktopServices::openUrl(QUrl ("http://labrococo.dis.uniroma1.it/dokuwiki/doku.php?id=tut:profiling#known_bugs"));
}

void MainWindow::moreInfo()
{
        QDesktopServices::openUrl(QUrl ("http://labrococo.dis.uniroma1.it/dokuwiki/doku.php?id=tut:profiling#information_about_the_profiling_file"));
}

void MainWindow::about() {
    AboutDialog ab;
    ab.setWindowTitle(tr("About %1").arg(qApp->applicationName()));
    ab.exec();
}

void MainWindow::loadFile(const QString &fileName) {
    if (fileName.isNull())
        open();
    closeFile();
    db = QSqlDatabase::addDatabase("QSQLITE", SESSION_NAME + QDateTime::currentDateTime().toString());
    db.setDatabaseName(fileName);
    if (!db.open())
    {
        QMessageBox::critical(this, tr("Error"), tr("Database Error.\n%1").arg(db.lastError().text()));
        return;
    }
    QSqlQuery q("SELECT * FROM sqlite_master WHERE 1=2", db);
    myLog->refreshLog("", "SELECT * FROM sqlite_master WHERE 1=2");
    if (!q.exec()) {
        QMessageBox::critical(this, tr("Error"), tr("Database Error.\n%1 is probably not an sqlite database").arg(fileName));
        return;
    }
    curFile = fileName;
    ui->statusBar->showMessage(tr("Opened file: %1").arg(fileName), 5000);
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();
    settings.setValue("recentFileList", files);
    updateRecentFileActions();
    model = new SqlTableModel(this, db);
    this->setWindowTitle(tr("%1 - %2").arg(qApp->applicationName()).arg(strippedName(fileName)));
    databaseActions->setEnabled(true);
    tableActions->setEnabled(true);
    indexActions->setEnabled(true);
    ui->tableTab->setEnabled(true);
    ui->databaseTab->setEnabled(true);
    ui->queryTab->setEnabled(true);
    ui->resultTab->setEnabled(true);
    reloadDbList();
}

void MainWindow::closeFile() {
    curFile = "";
    databaseActions->setEnabled(false);
    tableActions->setEnabled(false);
    indexActions->setEnabled(false);
    ui->lstStructure->clear();
    ui->cboTables->clear();
    this->setWindowTitle(qApp->applicationName());
    ui->txtQuery->setText("");
    ui->txtSearch->setText("");
    ui->tableTab->setEnabled(false);
    ui->databaseTab->setEnabled(false);
    ui->queryTab->setEnabled(false);
    ui->resultTab->setEnabled(false);
    SqlQueryModel *dummyModel = new SqlQueryModel(this);
    ui->tblData->setModel(dummyModel);
    ui->tblQueryResult->setModel(dummyModel);
    tableList.clear();
}

void MainWindow::reloadDbList() {
    ui->lstStructure->clear();
    ui->cboTables->clear();
    tableList.clear();
    indexList.clear();
    viewList.clear();
    //Show Tables
    QSqlQuery tableQuery("SELECT name, sql FROM sqlite_master WHERE type='table';", db);
    myLog->refreshLog("", "SELECT name, sql FROM sqlite_master WHERE type='table';");    
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->lstStructure);
    rootItem->setText(0, curFile);
    if (tableQuery.exec()) {
        while (tableQuery.next()) {
            QTreeWidgetItem *tableItem = new QTreeWidgetItem(rootItem);
            tableItem->setText(0, tableQuery.record().value("name").toString());
            tableItem->setText(1, "Table");
            tableItem->setText(3, tableQuery.record().value("sql").toString());
            tableList.insert(tableList.count(), tableItem->text(0));
            ui->cboTables->insertItem(ui->cboTables->count(), tableItem->icon(0), tableItem->text(0));
            QSqlQuery columnQuery(QString("PRAGMA TABLE_INFO(%1);").arg(tableItem->text(0)), db);
            myLog->refreshLog("", QString("PRAGMA TABLE_INFO(%1);").arg(tableItem->text(0)));
            if (columnQuery.exec()) {
                while (columnQuery.next()) {
                    QTreeWidgetItem *childItem = new QTreeWidgetItem(tableItem);
                    childItem->setText(0, columnQuery.record().value("name").toString());
                    childItem->setText(1, "Field");
                    childItem->setText(2, columnQuery.record().value("type").toString());
                }
            }
        }
    }
    //Show Indexes
    QSqlQuery indexQuery("SELECT name, sql FROM sqlite_master WHERE type='index';", db);
    myLog->refreshLog("", ("SELECT name, sql FROM sqlite_master WHERE type='index';"));
    if (indexQuery.exec()) {
        while (indexQuery.next()) {
            QTreeWidgetItem *indexItem = new QTreeWidgetItem(rootItem);
            indexItem->setText(0, indexQuery.record().value("name").toString());
            indexItem->setText(1, "Index");
            indexItem->setText(3, indexQuery.record().value("sql").toString());
            indexList.insert(indexList.count(), indexItem->text(0));
        }
    }
    //show views
    QSqlQuery viewQuery("SELECT name, sql FROM sqlite_master WHERE type='view';", db);
    myLog->refreshLog("", ("SELECT name, sql FROM sqlite_master WHERE type='view';"));
    if (viewQuery.exec()) {
        while (viewQuery.next()) {
            QTreeWidgetItem *viewItem = new QTreeWidgetItem(rootItem);
            viewItem->setText(0, viewQuery.record().value("name").toString());
            viewItem->setText(1, "View");
            viewItem->setText(3, viewQuery.record().value("sql").toString());
            viewList.insert(viewList.count(), viewItem->text(0));
            ui->cboTables->insertItem(ui->cboTables->count(), viewItem->icon(0), viewItem->text(0));
        }
    }
    rootItem->setExpanded(true);
 }

void MainWindow::openRecentFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

void MainWindow::updateRecentFileActions() {
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    QStringList files = settings.value("recentFileList").toStringList();
    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);
    for (int i = 0; i < numRecentFiles; ++i) {
        QString fNum = QString::number(i + 1);
        if (i < 9)
            fNum.prepend(" ");
        QString text = tr("%1 %2").arg(fNum).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setStatusTip(files[i]);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);
}

QString MainWindow::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::on_cmdNewRecord_clicked()
{
    model->insertRows(model->rowCount(), 1);
    ui->tblData->resizeRowsToContents();
}

void MainWindow::on_cmdDeleteRecord_clicked()
{
    if(model)
    {
        model->removeRows(ui->tblData->currentIndex().row(), 1);
        ui->tblData->resizeRowsToContents();
    }
}

void MainWindow::on_cmdSaveChanges_clicked()
{
    ui->tblData->selectRow(ui->tblData->currentIndex().row());
    if (!model->submitAll())
    {
        int ret = QMessageBox::question(this, qApp->applicationName(),
                                        tr("There is a pending transaction in progress. That cannot be commited now."\
                                           "\nError: %1\n"\
                                           "Perform rollback?").arg(model->lastError().text()),
                                        QMessageBox::Yes, QMessageBox::No);
        if(ret == QMessageBox::Yes)
            model->revertAll();
        model->setPendingTransaction(false);
        return;
    }
    model->setPendingTransaction(false);
    ui->tblData->resizeRowsToContents();
}

void MainWindow::on_cmdExecuteQuery_clicked()
{
    SqlQueryModel *dummyModel = new SqlQueryModel(this);
    ui->tblQueryResult->setModel(dummyModel);
    QString query = ui->txtQuery->toPlainText();
    myLog->refreshLog("user", query);
    if (!query.isEmpty()) {
        SqlQueryModel *queryModel = new SqlQueryModel(this);
        queryModel->setQuery(query, db);
        if (!queryModel->lastError().isValid()) {
            //No Error
            ui->tblQueryResult->setModel(queryModel);
            ui->tblQueryResult->resizeColumnsToContents();
            ui->tblQueryResult->resizeRowsToContents();
            ui->tabWidget->setCurrentIndex(3);
            emit statusMessage(tr("Query OK."));
        } else {
            //Error occurred
            emit statusMessage(queryModel->lastError().text());
        }
    }
}

void MainWindow::newTable() {
    CreateTableDialog createNewTable;
    createNewTable.db = db;
    if (createNewTable.exec()) {
        reloadDbList();
    }
}

void MainWindow::editTable() {
    QString tableName = getTable();
    if (tableName != "") {
        CreateTableDialog changeTable;
        changeTable.db = db;
        changeTable.startAlterTable(tableName);
        if (changeTable.exec())
            reloadDbList();
    }
}

void MainWindow::dropTable() {
    QString tableName = getTable();
    if (tableName != "") {
        if (QMessageBox::question(this, tr("Confirm Drop"), tr("Are you sure you want to delete this table?\nCAUTION!!!You will lose all data."), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
            return;
        QString dropSQL = QString("drop table '%1'").arg(tableName);
        db.exec(dropSQL);
        if (!db.lastError().isValid()) {
            reloadDbList();
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Drop Table Error:\n%1").arg(db.lastError().text()));
        }
    }
}

QString MainWindow::getTableNView() {
    SelectObjectDialog selectTable(this, tr("Select an object"), tr("Select a table or view"));
    QStringList allItems;
    allItems  << tableList << viewList;
    selectTable.setItems(allItems);
    QString tableName = "";
    if (selectTable.exec()) {
        tableName = selectTable.returnValue();
    }
    return tableName;
}

QString MainWindow::getTable() {
    SelectObjectDialog selectTable(this, tr("Select a table"), tr("Select a table"));
    selectTable.setItems(tableList);
    QString tableName = "";
    if (selectTable.exec()) {
        tableName = selectTable.returnValue();
    }
    return tableName;
}

QString MainWindow::getIndex() {
    SelectObjectDialog selectIndex(this, tr("Select an Index"), tr("Select an Index"));
    selectIndex.setItems(indexList);
    QString indexName = "";
    if (selectIndex.exec()) {
        indexName = selectIndex.returnValue();
    }
    return indexName;
}

void MainWindow::dropIndex() {
    QString indexName = getIndex();
    if (indexName != "") {
        if (QMessageBox::question(this, tr("Confirm Drop"), tr("Are you sure you want to delete this index?\nCAUTION!!!This action cannot be undone."), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
            return;
        QString dropSQL = QString("DROP index '%1'").arg(indexName);
        db.exec(dropSQL);
        myLog->refreshLog("", dropSQL);
        if (!db.lastError().isValid()) {
            reloadDbList();
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Drop Table Error:\n%1").arg(db.lastError().text()));
        }
    }
}

void MainWindow::createIndex() {
    QString tableName = getTable();
    if (tableName != "") {
        CreateIndexDialog *newIndex = new CreateIndexDialog(this);
        connect (newIndex, SIGNAL(emitLog(QString,QString)), myLog, SLOT(refreshLog(QString,QString)));
        newIndex->db = db;
        newIndex->setTableName(tableName);
        if (newIndex->exec())
            reloadDbList();
    }
}

void MainWindow::showPreferences() {
    PreferencesDialog pref;
    if (pref.exec())
        preferencesChanged();
}

void MainWindow::preferencesChanged() {
    //Preferences not yet implemented
    //PreferencesDialog pref;    
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasFormat("text/uri-list") )
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if( urls.isEmpty() )
        return;
    QString fileName = urls.first().toLocalFile();
    if( !fileName.isEmpty())
        loadFile(fileName);
}

void MainWindow::exportDatabaseAsSQL() {
    QString exportFileName = QFileDialog::getSaveFileName(this, tr("New SQL File"), "", tr("SQL File (*.sql);;All Files (*.*)"));
    if (exportFileName.size() > 0) {
        QFile f(exportFileName);
        if (!f.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this, qApp->applicationName(), tr("Could not create export file.\n%1").arg(f.errorString()));
        } else {
            QTextStream out(&f);
            QString sql ="";
            QSqlQuery qu("SELECT name FROM sqlite_master WHERE type='table'", db);
            while (qu.next()) {
                QString tableName = qu.value(0).toString();
                sql += "DROP TABLE IF EXISTS `" + tableName + "`;\n";
                sql += getTableCreateSQL(tableName);
                sql += "\n";
                sql += getTableInsertSQL(tableName);
            }
            //sql += "COMMIT;";
            out << sql;
            f.close();
            QMessageBox::information(this, qApp->applicationName(), tr("Export completed"));
        }
    }
}

void MainWindow::importDatabaseFromSQL() {
    if (curFile.isEmpty()) {
        newFile();
    }
    QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SQL Import"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    ui->txtQuery->setText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    db.exec(ui->txtQuery->toPlainText());
    ui->txtQuery->setText("");
    if (db.lastError().isValid())
        QMessageBox::warning(this, tr("SQL Import"), tr("Error:\n%1").arg(db.lastError().text()));
    QMessageBox::information(this, tr("SQL Import"), tr("SQL File imported successfully"));
    reloadDbList();
}

void MainWindow::exportTableToCSV() {
    QString tableName = getTableNView();
    QString exportFileName = QFileDialog::getSaveFileName(this, tr("New Log File"), "", tr("LOG Files (*.log);;All Files (*.*)"));
   // char quote = '';
    char deliminiter = ' ';
    QString endline = "\n";
    if (!exportFileName.isEmpty()) {
        QFile f(exportFileName);
        if (f.open(QIODevice::WriteOnly)) {
            QTextStream out(&f);
            QString header = "";
            QSqlQuery headerQu("PRAGMA TABLE_INFO(`" + tableName + "`)", db);
            while (headerQu.next()) {
           //     header +=  quote + headerQu.record().value("name").toString() + quote + deliminiter;
                header +=  headerQu.record().value("name").toString() + deliminiter;
            }
            header = header.remove(header.size() - sizeof(deliminiter), sizeof(deliminiter));
            header += endline;
            out << header;
            QString rowText = "";
            QSqlQuery qu("SELECT * FROM `" + tableName + "`", db);
            while (qu.next()) {
                QSqlRecord rec = qu.record();
                rowText = "";
                for (int i = 0; i < rec.count(); ++i) {
                   // rowText += quote + rec.value(i).toString() + quote + deliminiter;
                    rowText += rec.value(i).toString() + deliminiter;
                }
                rowText = rowText.remove(rowText.size() - sizeof(deliminiter), sizeof(deliminiter));
                rowText += endline;
                out << rowText;
            }
            f.close();
            QMessageBox::information(this, tr("Log Export"), tr("Export succeeded."));
        } else {
            QMessageBox::warning(this, tr("Log Export"), tr("Export error.\n%1").arg(f.errorString()));
        }
    }
}

void MainWindow::importTableFromCSV() {
    QString wFile = QFileDialog::getOpenFileName(this, tr("Select a Profiling Log File"), QDir::currentPath(), tr("LOG Files (*.log);;All Files (*.*)"));
    if (QFile::exists(wFile))
    {
        ImportCSVDialog csv(this, db, wFile);
        if (csv.exec()) {
            reloadDbList();
            QMessageBox::information(this, qApp->applicationName(), tr("Import completed" ));
        }
    }
}

void MainWindow::compactDb() {
    //Not Implemented
}

void MainWindow::searchAll() {
    if (ui->txtSearch->text().isEmpty())
    {
        QMessageBox::warning(this, qApp->applicationName(), tr("Please enter search text."));
        return;
    }
    QString search = ui->txtSearch->text();
    QString searchQuery = "SELECT * FROM `" + ui->cboTables->currentText() + "` WHERE ";
    QSqlQuery qu("PRAGMA TABLE_INFO(" + ui->cboTables->currentText() + ")", db);
    myLog->refreshLog("", "PRAGMA TABLE_INFO(" + ui->cboTables->currentText() + ")");
    while (qu.next()) {
        QString colName = qu.record().value("name").toString();
        searchQuery.append("`" + colName + QString("` LIKE '\%%1\%' OR ").arg(search));
    }
    searchQuery = searchQuery.remove(searchQuery.size() - 4, 4);
    doSearch(searchQuery);
}

void MainWindow::advancedSearch() {
    AdvancedSearchDialog adv(this, db, ui->cboTables->currentText());
    if (adv.exec()) {
        QString searchQuery = adv.searchQuery;
        doSearch(searchQuery);
    }
}

void MainWindow::doSearch(QString searchQuery) {
    myLog->refreshLog("", searchQuery);
    SqlQueryModel *queryModel = new SqlQueryModel(this);
    queryModel->setQuery(searchQuery, db);
    if (!queryModel->lastError().isValid()) {
        ui->tblData->setModel(queryModel);
        ui->tblData->resizeRowsToContents();
        ui->cmdDeleteRecord->setEnabled(false);
        ui->cmdNewRecord->setEnabled(false);
        ui->cmdSaveChanges->setEnabled(false);
    } else {
        QMessageBox::critical(this, tr("Search Error"), queryModel->lastError().text());
    }

}

void MainWindow::showTable(QString tableName) {
    model->setSchema("main");
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    ui->tblData->setModel(model);
    ui->cmdDeleteRecord->setEnabled(true);
    ui->cmdNewRecord->setEnabled(true);
    ui->tblData->resizeRowsToContents();
    ui->cmdDeleteRecord->setEnabled(true);
    ui->cmdNewRecord->setEnabled(true);
    ui->cmdSaveChanges->setEnabled(true);
}

void MainWindow::showTable() {
    showTable(ui->cboTables->currentText());
}

QString MainWindow::getTableCreateSQL(QString tableName) {
    QSqlQuery qu("SELECT sql FROM sqlite_master WHERE type='table' AND name = '" + tableName + "'", db);
    if (!qu.lastError().isValid()) {
        if (qu.next()) {
            QString sql = qu.value(0).toString();
            return sql + ";";
        } else {
            QMessageBox::warning(this, tr("Export Error"), tr("Error:\n%1").arg(qu.lastError().text()));
            return "";
        }
    } else {
        QMessageBox::warning(this, tr("Export Error"), qu.lastError().text());
        return "";
    }
}

QString MainWindow::getTableInsertSQL(QString tableName) {
    QString sql = "";
    QSqlQuery qu("SELECT * FROM `" + tableName + "`", db);
    QSqlQuery qu2("PRAGMA TABLE_INFO(`" + tableName + "`);", db);
    QStringList types;
    while (qu2.next()) {
        types.insert(types.count(), qu2.record().value("type").toString());
    }
    if (qu.lastError().isValid())
        return "";
    while (qu.next()) {
        QSqlRecord rec = qu.record();
        sql += "INSERT INTO " + tableName + " VALUES (";
        for (int i = 0; i < rec.count(); ++i) {
            QString colData = rec.value(i).toString();
            colData = colData.replace("'", "''");
            colData = "'" + colData + "'";
            QString type = types.at(i);
            if (type.toLower().contains("int"))
                colData = rec.value(i).toString();
            sql += colData + ", ";
        }
        sql = sql.remove(sql.size() - 2, 2);
        sql += ");\n";
    }
    return sql;
}

void MainWindow::changeEvent(QEvent *event)  {
    if (event != 0) {
        switch (event->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        case QEvent::LocaleChange:
            {
                QString locale = QLocale::system().name();
                locale.truncate(locale.lastIndexOf('_'));
            }
            break;
        default:
            break;
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::newView() {
    CreateViewDialog createView(this);
    createView.db = db;
    if (createView.exec()) {
        reloadDbList();
    }
}

void MainWindow::modifyView() {
    QString viewName = getView();
    CreateViewDialog editView(this);
    editView.db = db;
    editView.setViewName(viewName);
    if (editView.exec()) {
        reloadDbList();
    }
}

void MainWindow::deleteView() {
    QString viewName = getView();
    if (viewName == "")
        return;
    if (QMessageBox::question(this, tr("Confirm Delete"), tr("Are you sure you want to remove the <b>%1</b> view?").arg(viewName), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        db.exec(QString("DROP VIEW %1").arg(viewName));
        if (!db.lastError().isValid()) {
            reloadDbList();
        } else {
            QMessageBox::critical(this, tr("Error"), tr("There was an error while deleting the view.\n%1").arg(viewName));
        }
    }
}

QString MainWindow::getView() {
    SelectObjectDialog selectView(this, tr("Select a View"), tr("Select a View"));
    selectView.setItems(viewList);
    QString viewName = "";
    if (selectView.exec()) {
        viewName = selectView.returnValue();
    }
    return viewName;
}

void MainWindow::openExempleWindow()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Load Example");
    msgBox.setText("Before pressing LOAD and continue with the load of the example PLEASE check if in the application current folder (directory) there exist the file profiling.sqlitedb. If the file is not present then the DataBase will not be loaded.");
    msgBox.setInformativeText("Do you want to continue?");
    QAbstractButton *myYesButton = msgBox.addButton(trUtf8("LOAD"), QMessageBox::YesRole);
    QAbstractButton *myNoButton = msgBox.addButton(trUtf8("Cancel"), QMessageBox::NoRole);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
    if(msgBox.clickedButton() == myYesButton)
    {
        actionLoadanExample();
    }
    if(msgBox.clickedButton() == myNoButton)
    {
    }
}

void MainWindow::openImportWindow()
{
    QMessageBox msgBox;
    QSpacerItem* horizontalSpacer = new QSpacerItem(700, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    msgBox.setWindowTitle("Import Table");
    msgBox.setText("Before pressing YES and load the log file PLEASE add as the first line in the file :\n "
                   "'Timestamp WrongValue ThreadTimestamp ThreadID EventType EventSubject Other'.\n"
                   "Or by using the script inside the folder 'otherfiles'. command 'sh script.sh'\n"
                   "In this way you will have names for the table.");
    msgBox.setInformativeText("Do you want to continue?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    msgBox.setIcon(QMessageBox::Information);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Yes:
          // Save was clicked
       importTableFromCSV();
          break;
      case QMessageBox::No:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }
}

void MainWindow::on_loadQuery_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open SQL File"), "",
        tr("SQL Files (*.sql);;Text Files (*.txt)"));
    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        ui->txtQuery->setText(in.readAll());
        file.close();
    }
}

void MainWindow::on_saveQuery_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
    tr("SQL Files (*.sql);;Text Files (*.txt)"));
    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
        } else {
            QTextStream stream(&file);
            stream << ui->txtQuery->toPlainText();
            stream.flush();
            file.close();
        }
    }
}

void MainWindow::on_clearButton_clicked()
{
    ui->txtQuery->clear();
}

void MainWindow::on_searchButton_clicked()
{
    QString searchString = ui->searchLine->text();
    ui->txtQuery->find(searchString, QTextDocument::FindWholeWords);
}

void MainWindow::on_actionoutBallPercept_clicked()
{
        QFile inputFile(":SQL/Perception/outballPercept.sql");
        inputFile.open(QIODevice::ReadOnly);
        QTextStream in(&inputFile);
        QString line = in.readAll();
        inputFile.close();
        ui->txtQuery->setPlainText(line);
        QTextCursor cursor = ui->txtQuery->textCursor();
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionoutBallInImage_clicked()
{
    QFile inputFile(":SQL/Perception/outballInImage.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionoutGoalMineSeen_clicked()
{
    QFile inputFile(":SQL/Perception/outgoalMineSeen.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionoutGoalMineLeftPost_clicked()
{
    QFile inputFile(":SQL/Perception/outgoalMineLeftPost.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionoutGoalOpponentSeen_clicked()
{
    QFile inputFile(":SQL/Perception/outgoalOpponentSeen.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionoutGoalOpponentLeftPost_clicked()
{
    QFile inputFile(":SQL/Perception/outgoalOpponentLeftPost.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actioninGoalMineSeen_clicked()
{
    QFile inputFile(":SQL/Executor/ingoalMineSeen.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actioninGoalMineLeftPost_clicked()
{
    QFile inputFile(":SQL/Executor/ingoalMineLeftPost.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actioninGoalOpponentSeen_clicked()
{
    QFile inputFile(":SQL/Executor/ingoalOpponentSeen.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actioninCoordinateBallField_clicked()
{
    QFile inputFile(":SQL/Executor/incoordinateBallField.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actioninCoordinateBallImage_clicked()
{
    QFile inputFile(":SQL/Executor/incoordinateBallImage.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actioninGoalOpponentLeftPost_clicked()
{
    QFile inputFile(":SQL/Executor/ingoalOpponentLeftPost.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionModuleDuration_clicked()
{
    QFile inputFile(":SQL/moduleDuration.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionPropLockUnlock_clicked()
{
    QFile inputFile(":SQL/propLockedUnlocked.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionPropReadWritten_clicked()
{
    QFile inputFile(":SQL/propReadWritten.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionFrequencyModule_clicked()
{
    QFile inputFile(":SQL/frequencyModule.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::on_actionAll_clicked()
{
    QFile inputFile(":SQL/all.sql");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();
    ui->txtQuery->setPlainText(line);
    QTextCursor cursor = ui->txtQuery->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void MainWindow::actionLoadanExample()
{
    db = QSqlDatabase::addDatabase("QSQLITE", SESSION_NAME + QDateTime::currentDateTime().toString());
    QString path(QDir::current().path());
    path.append(QDir::separator()).append("profiling.sqlitedb");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    if (!db.open())
    {
        QMessageBox::critical(this, tr("Error"), tr("Database Error.\n%1 the database is not in the same").arg(db.lastError().text()));
        return;
    }
    QSqlQuery q("SELECT * FROM sqlite_master WHERE 1=2", db);
    myLog->refreshLog("", "SELECT * FROM sqlite_master WHERE 1=2");
    if (!q.exec()) {
        QMessageBox::critical(this, tr("Error"), tr("Database Error.\n%1 is probably not an sqlite database").arg(path));
        return;
    }
    curFile = path;
    ui->statusBar->showMessage(tr("Opened file: %1").arg(path), 5000);
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(path);
    files.prepend(path);
    while (files.size() > MaxRecentFiles)
        files.removeLast();
    settings.setValue("recentFileList", files);
    updateRecentFileActions();
    model = new SqlTableModel(this, db);
    this->setWindowTitle(tr("%1 - %2").arg(qApp->applicationName()).arg(strippedName(path)));
    databaseActions->setEnabled(true);
    tableActions->setEnabled(true);
    indexActions->setEnabled(true);
    ui->tableTab->setEnabled(true);
    ui->databaseTab->setEnabled(true);
    ui->queryTab->setEnabled(true);
    ui->resultTab->setEnabled(true);
    reloadDbList();
    on_actionAll_clicked();
}
