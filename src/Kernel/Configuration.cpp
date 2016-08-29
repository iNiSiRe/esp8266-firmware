//
// Created by inisire on 29.08.16.
//

#include "Configuration.h"

bool Configuration::load()
{
    DynamicJsonBuffer buffer;

    if (!fileExist(FILE)) {
        return false;
    }

    int size = fileGetSize(FILE);
    char * jsonString = new char[size + 1];
    fileGetContent(FILE, jsonString, size + 1);
    JsonObject& root = buffer.parseObject(jsonString);

    ssid = root["ssid"].asString();
    password = root["password"].asString();
    uid = root["uid"].as<long>();

    delete[] jsonString;

    return true;
}

bool Configuration::save() {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["ssid"] = ssid.c_str();
    root["password"] = password.c_str();
    root["uid"] = String(uid);

    String rootString;
    root.printTo(rootString);
    fileSetContent(FILE, rootString);

    return true;
}
