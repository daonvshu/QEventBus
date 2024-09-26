cmake_minimum_required (VERSION 3.12)

project("QEventBus")

option(QEVENTBUS_BUILD_TEST "enable QEventBus test projects" OFF)

add_subdirectory(src)

if (QEVENTBUS_BUILD_TEST)
    add_subdirectory(test)
endif ()