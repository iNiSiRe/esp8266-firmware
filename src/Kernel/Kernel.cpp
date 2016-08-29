#include <WebSocketClient/WebSocketClient.h>
#include "Kernel.h"

Kernel::Kernel(Configuration * configuration)
{
    this->configuration = configuration;

    // Initialize WIFI
    WifiAccessPoint.enable(false);
    WifiStation.enable(true);

    client = new WebSocketClient();
    client->onReady = WebSocketClientDelegate(&Kernel::onSocketConnectSuccess, this);
    client->onData = WebSocketDataDelegate(&Kernel::onSocketData, this);
}

void Kernel::connect(bool save)
{
    WifiStation.config(configuration->ssid, configuration->password);
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
    client->connect("ws.tumbler.one", 8000);

    Timer * timer = new Timer();
    timer->initializeMs(10000, TimerDelegate(&Kernel::onSocketStatusCheck, this)).start();
}

void Kernel::onSocketConnectSuccess()
{
    debugf("Socket is ready, register module\n");

    DynamicJsonBuffer buffer;
    JsonObject & root = buffer.createObject();
    root["action"] = (int) ACTION_REGISTER;
    root["uid"] = configuration->uid;

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

        case ACTION_UPDATE:
            HttpFirmwareUpdate * update = new HttpFirmwareUpdate();
            update->addItem(0x0000, "http://smart-home.tumbler.one/flash.bin");
            update->start();
            break;

        default:
            debugf("Kernel::onSocketData ==> No action");
            break;
    }
}

void Kernel::onSocketStatusCheck() {
    switch (client->getConnectionState()) {
        case eTCS_Ready:
            debugf("Socket ready");
            break;
        case eTCS_Connecting:
            debugf("Socket connecting");
            break;
        case eTCS_Connected:
            debugf("Socket connected");
            break;
        case eTCS_Successful:
            debugf("Socket successful");
            break;
        case eTCS_Failed:
            debugf("Socket failed");
            break;
    }
}
