#include "LightUnit.h"

void LightUnit::dim(uint32 value) {
    debugf("LightUnit::handle => Dim light '%s' to '%d' ", name.c_str(), value);
    driver->analogWrite(pin, value);
}

LightUnit::LightUnit(String name, HardwarePWM *driver, uint8_t pin) : Unit(name, "light", "control", pin)
{
    pinMode(pin, OUTPUT);
    this->driver = driver;
}

void LightUnit::handle(JsonObject &root)
{
    if (!root.containsKey("value")) {
        debugf("LightUnit::handle => JSON request hasn't key 'value'");
        return;
    }

    float value = (float) root["value"];

    dim((uint32) round(value * 22222.0));
}
