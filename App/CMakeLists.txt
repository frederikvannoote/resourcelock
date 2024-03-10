set(HEADERS
    feature.h
    resource.h)

set(SOURCES
    main.cpp
    feature.cpp
    resource.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(App ${SOURCES} ${HEADERS})
target_link_libraries(App PRIVATE LockLib Qt6::Core)
