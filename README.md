# Ubiquiti UMR Industrial / CalTopo GPS relay

## Build Environment
Initially developed with:
- Arduino IDE 2.3.7
- Board: uPesy ESP32 Wroom DevKit
- Additional Installed Libraries:
    - ArduinoJson

## Setup
If the device does not have wi-fi configuration, or if you press and hold the Boot button within a second of power-on reset, the device will start its own Wi-Fi network. Connect to the network and then point a web browser at the device.

If the device is already configured, you can change settings using the built-in web server at the DHCP assigned address.

- **SSID**: The name of the truck wi-fi network.
- **Password**: The wi-fi password.
- **Device ID**: The name of the device as it will appear in the CalTopo team admin Trackables > Trackable Devices table.
- **Modem IP**: The LAN IP address of the UMR Industrial LTE modem.
- **Modem Password**: The local admin password for the modem.
- **CalTopo Trackable Key**: A valid Connect Key from the CalTopo team admin Trackables > Access URLs table.

All settings must be entered when making config updates.