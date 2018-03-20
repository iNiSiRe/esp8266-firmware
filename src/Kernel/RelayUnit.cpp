#include "RelayUnit.h"

RelayUnit::RelayUnit(const String &name, const uint8_t &pin) : Unit(name, "relay", "control", pin)
{
    pinMode(pin, OUTPUT);
}

void RelayUnit::handle(JsonObject &root)
{
    if (!root.containsKey("value")) {
                debugf("RelayUnit::handle => JSON request hasn't key 'value'");
        return;
    }

    int value = (int) root["value"];

    debugf("RelayUnit::handle => Switch relay '%s' to '%d' %%", name.c_str(), value);

    if (value == 1) {
        digitalWrite(pin, HIGH);
    } else {
        digitalWrite(pin, LOW);
    }
}
