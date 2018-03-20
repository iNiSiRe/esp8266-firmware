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
    client->connect("192.168.31.135", 8000);
}

void Kernel::onSocketConnectSuccess()
{
    debugf("Socket is ready, register module\n");

    DynamicJsonBuffer buffer;
    JsonObject & root = buffer.createObject();
    root["action"] = (int) ACTION_LOGIN;
    root["id"] = "1";

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

            if (root["type"] == "digital") {
                digitalWrite(root["pin"], root["value"]);
            }

            break;

        default:
            debugf("Kernel::onSocketData ==> No action");
            break;
    }
}