QT       += core gui uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ioServiceThread.cpp \
    main.cpp \
    server.cpp \
    servercontroller.cpp

HEADERS += \
    connection.h \
    ioServiceThread.h \
    server.h \
    servercontroller.h

FORMS += \
    servercontroller.ui

INCLUDEPATH += \
    C:/ProgrammingLibs/boost_1_78_0

LIBS += \
    -lwsock32 \
    -lws2_32 \
    -l:libboost_thread-mgw11-mt-x64-1_78.a \
    -LC:\ProgrammingLibs\boost_1_78_0\stage\lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
