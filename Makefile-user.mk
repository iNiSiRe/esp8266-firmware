## Local build configuration
## Parameters configured here will override default and ENV values.
## Uncomment and change examples:

## Add your source directories here separated by space
MODULES = app src/Kernel src/WebSocketClient
EXTRA_INCDIR = src

ESP_HOME = /opt/esp-open-sdk
SMING_HOME = /opt/Sming/Sming

## COM port parameter is reqruied to flash firmware correctly.
## Windows: 
# COM_PORT = COM3

## MacOS / Linux:
COM_PORT = /dev/ttyUSB0

## Com port speed
# COM_SPEED	= 115200

## Configure flash parameters (for ESP12-E and other new boards):
# SPI_MODE = dio

## SPIFFS options

DISABLE_SPIFFS = 1

# SPIFF_FILES = files

