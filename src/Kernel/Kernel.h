#ifndef SMARTHOME_KERNEL_H
#define SMARTHOME_KERNEL_H

#include <SmingCore/SmingCore.h>
#include "Unit.h"
#include "LightUnit.h"
#include <WebSocketClient/WebSocketClient.h>
#include <WString.h>

enum KernelAction {
    ACTION_REGISTER = 1,
    ACTION_CONTROL = 2,
    ACTION_LOGIN = 4
};

class Kernel {

public:
    Kernel();
    void connect(String ssid, String password, bool save);
    void addUnit(Unit *unit);

private:
    Vector <Unit *> units;
    WebSocketClient * client;

    void onStationConnectSuccess();
    void onStationConnectFail();

    void onSocketConnectSuccess();
    void onSocketData(String message);
};

#endif //SMARTHOME_KERNEL_H
