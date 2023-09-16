QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    datasetloader.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    datasetloader.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Nextcloud/PC-Documents/Programier Zeug/Qt-Projekte/ViewNet/release/' -lViewNet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Nextcloud/PC-Documents/Programier Zeug/Qt-Projekte/ViewNet/debug/' -lViewNet

INCLUDEPATH += $$PWD/'../../../../Nextcloud/PC-Documents/Programier Zeug/Qt-Projekte/ViewNet'
DEPENDPATH += $$PWD/'../../../../Nextcloud/PC-Documents/Programier Zeug/Qt-Projekte/ViewNet'

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Nextcloud/PC-Documents/Programier Zeug/Qt-Projekte/GenNet/release/' -lGenNet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Nextcloud/PC-Documents/Programier Zeug/Qt-Projekte/GenNet/debug/' -lGenNet

INCLUDEPATH += $$PWD/'../../../../Nextcloud/PC-Documents/Programier Zeug/Qt-Projekte/GenNet'
DEPENDPATH += $$PWD/'../../../../Nextcloud/PC-Documents/Programier Zeug/Qt-Projekte/GenNet'
