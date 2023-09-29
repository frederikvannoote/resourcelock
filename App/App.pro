QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# ResourceLib dependency
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ResourceLib/release/ -lResourceLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ResourceLib/debug/ -lResourceLib
else:unix: LIBS += -L$$OUT_PWD/../ResourceLib/ -lResourceLib

INCLUDEPATH += $$PWD/../ResourceLib
DEPENDPATH += $$PWD/../ResourceLib

# LockLib dependency
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LockLib/release/ -lLockLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LockLib/debug/ -lLockLib
else:unix: LIBS += -L$$OUT_PWD/../LockLib/ -lLockLib

INCLUDEPATH += $$PWD/../LockLib
DEPENDPATH += $$PWD/../LockLib
