#ifndef SMARTHOME_UNIT_H
#define SMARTHOME_UNIT_H

#include <SmingCore/SmingCore.h>

class Unit
{
public:
    String name, category, type;

    Unit(String name, String category, String type, uint8_t pin);
    void on();
    void off();
    virtual void handle(JsonObject & root) = 0;

protected:
    uint8_t pin;
};

#endif //SMARTHOME_UNIT_H
