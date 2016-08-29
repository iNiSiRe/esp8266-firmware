#include <Kernel/LightUnit.h>
#include <Kernel/Configuration.h>
#include "Kernel/Kernel.h"

Timer timer;
uint8_t pins[1] = { 4 };
HardwarePWM driver(pins, 1);

void init() {

    // Initialize Serial
    Serial.begin(115200);
    Serial.systemDebugOutput(true);

    Configuration * configuration = new Configuration();
    if (!configuration->load()) {
        configuration->ssid = "Network";
        configuration->password = "1234567890";
        configuration->uid = random();
    }

    Kernel * kernel = new Kernel(configuration);
    kernel->connect(true);

    Serial.println("Init driver and light");

    LightUnit * light = new LightUnit("surface_light", &driver, 4);
    kernel->addUnit(light);
}