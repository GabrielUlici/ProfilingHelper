QT += sql
TEMPLATE = app
TARGET = ProfilingHelper
DEPENDPATH += . src
INCLUDEPATH += src

# Input
HEADERS += src/aboutdialog.h \
           src/addcolumndialog.h \
           src/advancedsearchdialog.h \
           src/createindexdialog.h \
           src/createtabledialog.h \
           src/database.h \
           src/highlighter.h \
           src/importcsvdialog.h \
           src/logdialog.h \
           src/mainwindow.h \
           src/preferences.h \
           src/preferencesdialog.h \
           src/selectobjectdialog.h \
           src/sqlmodels.h \
    src/importcsvthread.h \
    src/createviewdialog.h
FORMS += src/aboutdialog.ui \
         src/addcolumndialog.ui \
         src/advancedsearchdialog.ui \
         src/createindexdialog.ui \
         src/createtabledialog.ui \
         src/importcsvdialog.ui \
         src/logdialog.ui \
         src/mainwindow.ui \
         src/preferencesdialog.ui \
         src/selectobjectdialog.ui \
    src/createviewdialog.ui
SOURCES += src/aboutdialog.cpp \
           src/addcolumndialog.cpp \
           src/advancedsearchdialog.cpp \
           src/createindexdialog.cpp \
           src/createtabledialog.cpp \
           src/database.cpp \
           src/highlighter.cpp \
           src/importcsvdialog.cpp \
           src/logdialog.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/preferences.cpp \
           src/preferencesdialog.cpp \
           src/selectobjectdialog.cpp \
           src/sqlmodels.cpp \
    src/importcsvthread.cpp \
    src/createviewdialog.cpp
RESOURCES += src/resources.qrc
TRANSLATIONS += language/profilinghelper_en.ts \
                language/profilinghelper_ro.ts \
                language/profilinghelper_it.ts

win32 {
RC_FILE = winapp.rc
}

mac {
    ICON = profilinghelper.icns 
    QMAKE_INFO_PLIST = Info_mac.plist
     }

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc

OTHER_FILES +=


