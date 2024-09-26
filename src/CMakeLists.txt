project("QEventBus")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(CMAKE_DEBUG_POSTFIX "d")

add_library(${PROJECT_NAME}
    qeventbus.h
    qeventbus.cpp
    eventdispatcher.h
    eventdispatcher.cpp
)

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core)