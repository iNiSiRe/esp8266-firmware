cmake_minimum_required(VERSION 3.3)

project(SmartHome)

include_directories(
        src
        /esp/Sming/Sming/include
        /esp/Sming/Sming/
        /esp/Sming/Sming/system/include
        /esp/Sming/Sming/Wiring
        /esp/Sming/Sming/Libraries
        /esp/Sming/Sming/SmingCore
        /esp/esp-open-sdk/sdk/include
        /esp/Sming/Sming/rboot
        /esp/Sming/Sming/rboot/appcode
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        app/application.cpp
        src/Kernel/Kernel.cpp src/Kernel/Kernel.h
        src/Kernel/Unit.cpp src/Kernel/Unit.h
        src/Kernel/LightUnit.cpp src/Kernel/LightUnit.h
        src/WebSocketClient/WebSocketClient.cpp src/WebSocketClient/WebSocketClient.h)

add_executable(SmartHome ${SOURCE_FILES})