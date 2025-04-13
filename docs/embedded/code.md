# Embedded code

## ESP32-WROOM-DA Module (Installation Device)

The ESP32 acts as the installation device with auditive and visual output modules. It fetches environmental data from the sensor devices over HTTPS from the API and updates the LED strip and plays a sound based on the received weather data. It also acts as a secure HTTPS server and fetches an HTTPS POST Request to **NGINX Push Stream** which allows WebSocket to update the current index of which device is currently displayed.

[Source Code for inspection](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/installation)

### Key Responsibilities

- Uses WiFiManager for dynamic WiFi config to remove hardcoded credentials.
- Fetches latest sensor readings securely from the API (HTTPS GET).
- Rotates through every sensor reader every 5 seconds.
- Visualizes temperature on a WS2812B LED.
- Plays weather audio using a DFPlayer Mini.
- Hosts a self-signed HTTPS server.
- Pushes `currentIndex` updates to NGINX Push Stream (HTTPS POST).

### Libraries

| Library Name          | Version |
| --------------------- | ------- |
| `ArduinoJson`         | 7.3.1   |
| `WiFi`                | 3.1.3   |
| `Networking`          | 3.1.3   |
| `WiFiManager`         | 2.0.17  |
| `Update`              | 3.1.3   |
| `WebServer`           | 3.1.3   |
| `DNSServer`           | 3.1.3   |
| `FS`                  | 3.1.3   |
| `AsyncUDP`            | 3.1.3   |
| `FastLED`             | 3.9.14  |
| `NetworkClientSecure` | 3.1.3   |
| `DFRobotDFPlayerMini` | 1.0.6   |

### Board Manager

| Platform | Version |
| -------- | ------- |
| `ESP32`  | 3.2.0   |

## WEMOS D1 Mini (Reader Device)

The WEMOS D1 Mini acts as a **sensor reader** Device. It reads values from input sensors and sends them over HTTPS to the backend API. These devices are scalable since they register themselves via their MAC Addresses.

[Source Code for inspection](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/sensors)

### Key Responsibilities

- Registers itself to the Database via MAC address through the API (HTTPS POST).
- Uses WiFiManager for dynamic WiFi config to remove hardcoded credentials.
- Lights up an LED once setup is done.
- Reads and send data to API every 2 seconds.

### Libraries

| Library Name              | Version |
| ------------------------- | ------- |
| `Wire`                    | 1.0     |
| `Adafruit BMP085`         | 1.2.4   |
| `Adafruit BusIO`          | 1.17.0  |
| `BH1750`                  | 1.3.0   |
| `DHT sensor library`      | 1.4.6   |
| `WiFiManager`             | 2.0.17  |
| `ESP8266WiFi`             | 1.0     |
| `ESP8266WebServer`        | 1.0     |
| `DNSServer`               | 1.1.1   |
| `ArduinoJson`             | 7.3.1   |
| `SPI`                     | 1.0     |
| `Adafruit Unified Sensor` | 1.1.15  |

### Board Manager

| Platform  | Version |
| --------- | ------- |
| `ESP8266` | 3.1.2   |
