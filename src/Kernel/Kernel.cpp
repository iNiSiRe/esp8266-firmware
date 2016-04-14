#include <WebSocketClient/WebSocketClient.h>
#include "Kernel.h"

Kernel::Kernel()
{
    // Initialize WIFI
    WifiAccessPoint.enable(false);
    WifiStation.enable(true);

    client = new WebSocketClient();
    client->onReady = WebSocketClientDelegate(&Kernel::onSocketConnectSuccess, this);
    client->onData = WebSocketDataDelegate(&Kernel::onSocketData, this);
}

void Kernel::connect(String ssid, String password, bool save)
{
    WifiStation.config(ssid, password);
    WifiStation.waitConnection(ConnectionDelegate(&Kernel::onStationConnectSuccess, this), 20, ConnectionDelegate(&Kernel::onStationConnectFail, this));
}

void Kernel::addUnit(Unit *unit) {
    units.add(unit);
}

void Kernel::onStationConnectFail() {
    debugf("Connection fail");
}

void Kernel::onStationConnectSuccess() {
    debugf("Connected, IP: %s \n", WifiStation.getIP().toString().c_str());
    client->connect("192.168.1.109", 8000);
}

void Kernel::onSocketConnectSuccess()
{
    debugf("Socket is ready, register module\n");

    DynamicJsonBuffer buffer;
    JsonObject & root = buffer.createObject();
    root["action"] = (int) ACTION_REGISTER;
    root["room"] = "Room1";
    root["module"] = "Module1";

    JsonArray & units = buffer.createArray();
    for (int i = 0; i < this->units.size(); i++) {
        JsonObject & unit = buffer.createObject();
        unit["name"] = this->units[i]->name;
        unit["class"] = this->units[i]->category;
        unit["type"] = this->units[i]->type;
        units.add(unit);
    }
    root["units"] = units;

    String packet;
    root.printTo(packet);

    client->send(packet);
}

void Kernel::onSocketData(String message)
{
    debugf("Received data: %s\n", message.c_str());

    DynamicJsonBuffer buffer;
    JsonObject & root = buffer.parseObject(message);

    if (!root.success()) {
        debugf("JSON parse failed");
        return;
    }

    if (!root.containsKey("action")) {
        debugf("Bad JSON request: 'action' key is missing");
        return;
    }

    KernelAction action = (KernelAction) (atoi(root["action"].asString()));

    switch (action) {
        case ACTION_CONTROL:
            debugf("Kernel::onSocketData ==> Action control");
            for (int i = 0; i < units.size(); i++) {
                if (units.elementAt(i)->name == root["unit"]) {
                    Unit *unit = const_cast <Unit *> (units.elementAt(i));
                    unit->handle(root);
                }
            }
            break;

        default:
            debugf("Kernel::onSocketData ==> No action");
            break;
    }
}
