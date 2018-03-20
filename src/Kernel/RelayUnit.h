#ifndef SMARTHOME_RELAYUNIT_H
#define SMARTHOME_RELAYUNIT_H

#include <cstdint>
#include "Unit.h"

class RelayUnit : public Unit
{
public:
    RelayUnit(const String &name, const uint8_t &pin);
    void handle(JsonObject & root);
};

#endif //SMARTHOME_RELAYUNIT_H
