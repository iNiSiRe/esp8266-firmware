//#include <Kernel/LightUnit.h>
//#include "Kernel/Kernel.h"

#include <SmingCore/SmingCore.h>

void init() {
    // Initialize Serial
    Serial.begin(115200);
    Serial.systemDebugOutput(true);

    Serial.println("Test");

    WifiAccessPoint.enable(false);
    WifiStation.enable(true);
    WifiStation.config("Network", "f6d11bf3cd0c232");

//    Kernel * kernel = new Kernel;
//    kernel->connect("Network", "f6d11bf3cd0c232", true);

    Serial.println("Init driver and light");
}