#include "Unit.h"

Unit::Unit(String name, String category, String type, uint8_t pin) {
    this->name = name;
    this->category = category;
    this->type = type;
    this->pin = pin;
}

void Unit::on() {
    digitalWrite(pin, 255);
}

void Unit::off() {
    digitalWrite(pin, 0);
}