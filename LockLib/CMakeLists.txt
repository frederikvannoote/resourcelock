set(PUBLIC_HEADERS
    LockLib_global.h
    cancellationrequest.h
    readwritelock.h
    readwritelocker.h
)

set(PRIVATE_HEADERS
    cancellationrequest_p.h
    readwritelock_p.h
    readwritelocker_p.h
)

set(SOURCES
    cancellationrequest.cpp
    readwritelock.cpp
    readwritelocker.cpp
)

add_library(LockLib STATIC ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})

set_target_properties(LockLib PROPERTIES
    VERSION 1.0
    SOVERSION 1
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    PRIVATE_HEADER "${PRIVATE_HEADERS}"
)

target_include_directories(LockLib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)