#ifndef SMARTHOME_LIGHTUNIT_H
#define SMARTHOME_LIGHTUNIT_H

#include <SmingCore/SmingCore.h>
#include "Unit.h"

class LightUnit : public Unit {

protected:
    DriverPWM * driver;

public:
    LightUnit(String name, DriverPWM *driver, uint8_t pin);

    void dim(uint8_t value);
    void handle(JsonObject & root);
};


#endif //SMARTHOME_LIGHTUNIT_H
