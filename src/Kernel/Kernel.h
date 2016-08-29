#ifndef SMARTHOME_KERNEL_H
#define SMARTHOME_KERNEL_H

#include <SmingCore/SmingCore.h>
#include "Unit.h"
#include "LightUnit.h"
#include "Configuration.h"
#include <WebSocketClient/WebSocketClient.h>
#include <WString.h>

enum KernelAction {
    ACTION_REGISTER = 1,
    ACTION_CONTROL,
    ACTION_UPDATE
};

class Kernel {

public:
    Kernel(Configuration * configuration);
    void connect(bool save);
    void addUnit(Unit *unit);

private:
    Configuration * configuration;
    Vector <Unit *> units;
    WebSocketClient * client;

    void onStationConnectSuccess();
    void onStationConnectFail();

    void onSocketConnectSuccess();
    void onSocketData(String message);

    void onSocketStatusCheck();
};

#endif //SMARTHOME_KERNEL_H
