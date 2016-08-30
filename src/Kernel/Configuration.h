//
// Created by inisire on 29.08.16.
//

#ifndef SMARTHOME_CONFIGURATION_H
#define SMARTHOME_CONFIGURATION_H

#include <SmingCore/SmingCore.h>

class Configuration
{

private:
    const String FILE = "configuration.json";

public:
    String ssid, password, updateUrl, webSocketServer;
    int webSocketPort;
    long uid;

    bool load();
    bool save();
};


#endif //SMARTHOME_CONFIGURATION_H
