#ifndef SMARTHOME_LIGHTUNIT_H
#define SMARTHOME_LIGHTUNIT_H

#include <SmingCore/SmingCore.h>
#include "Unit.h"

class LightUnit : public Unit {

protected:
    HardwarePWM * driver;

public:
    LightUnit(String name, HardwarePWM *driver, uint8_t pin);

    void dim(uint32 value);
    void handle(JsonObject & root);
};


#endif //SMARTHOME_LIGHTUNIT_H
