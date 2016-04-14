#include <Kernel/LightUnit.h>
#include "Kernel/Kernel.h"

void init() {
    // Initialize Serial
    Serial.begin(115200);
    Serial.systemDebugOutput(true);

    Kernel * kernel = new Kernel;
    kernel->connect("Network", "1234567890", true);

    Serial.println("Init driver and light");
    DriverPWM * driver = new DriverPWM;
    LightUnit * light = new LightUnit("led_strip", driver, 4);
    kernel->addUnit(light);
}