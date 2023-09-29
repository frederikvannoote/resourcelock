TEMPLATE = lib
DEFINES += LOCKLIB_LIBRARY

CONFIG += c++11

SOURCES += \
    cancellationrequest.cpp \
    readwritelock.cpp \
    readwritelocker.cpp

HEADERS += \
    LockLib_global.h \
    cancellationrequest.h \
    cancellationrequest_p.h \
    readwritelock.h \
    readwritelock_p.h \
    readwritelocker.h \
    readwritelocker_p.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
