cmake_minimum_required(VERSION 3.3)

project(SmartHome)

include_directories(
        src
        /opt/Sming/Sming/include
        /opt/Sming/Sming/
        /opt/Sming/Sming/system/include
        /opt/Sming/Sming/Wiring
        /opt/Sming/Sming/Libraries
        /opt/Sming/Sming/SmingCore
        /opt/esp-open-sdk/sdk/include
        /opt/Sming/Sming/rboot
        /opt/Sming/Sming/rboot/appcode
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        app/application.cpp
        src/Kernel/Kernel.cpp src/Kernel/Kernel.h
        src/Kernel/Unit.cpp src/Kernel/Unit.h
        src/Kernel/LightUnit.cpp src/Kernel/LightUnit.h
        src/WebSocketClient/WebSocketClient.cpp src/WebSocketClient/WebSocketClient.h
        src/Kernel/RelayUnit.cpp src/Kernel/RelayUnit.h)

add_executable(SmartHome ${SOURCE_FILES})