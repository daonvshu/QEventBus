project("qeventbus")

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

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)

install(TARGETS ${PROJECT_NAME}
    EXPORT QEventBusTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/QEventBusConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/QEventBusConfig.cmake"
    INSTALL_DESTINATION lib/cmake/QEventBus
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/QEventBusConfig.cmake"
    DESTINATION lib/cmake/QEventBus
)

install(EXPORT QEventBusTargets
    FILE QEventBusTargets.cmake
    DESTINATION lib/cmake/QEventBus
)