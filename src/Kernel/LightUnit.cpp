#include "LightUnit.h"

void LightUnit::dim(uint8_t value) {
    driver->analogWrite(pin, value);
}

LightUnit::LightUnit(String name, DriverPWM *driver, uint8_t pin) : Unit(name, "light", "control", pin)
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

    int value = (int) root["value"];

    debugf("LightUnit::handle => Dim light '%s' to '%d' %%", name.c_str(), value);

    dim((uint8_t) round(value / 100 * 1024));
}
