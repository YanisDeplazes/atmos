# System Requirements

To build the embedded device, the following requirements from DLO must be fulfilled:

| Requirement ID# | Requirement                                                                                                                                                                   | MoSCoW | Compliant |
| --------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------ | --------- |
| EMBRQ#01        | The embedded device acts as a client and sends measured sensor data to the application backend over http or https.                                                            | MUST   | YES       |
| EMBRQ#02        | The embedded device also acts as a server and receives status messages from the application backend over http or https.                                                       | MUST   | YES       |
| EMBRQ#03        | The embedded device contains at least two types of input sensors (e.g. LDR, buttons, joystick, capacitive touch, etc.).                                                       | MUST   | YES       |
| EMBRQ#04        | The embedded device contains at least two types of visual and/or sensory outputs (e.g. LED, LED Matrix, 7-segment display, motor, servo, actuator, LCD-screen, buzzer, etc.). | MUST   | YES       |
| EMBRQ#05        | The embedded device uses the WifiManager for configuration of SSID and password (PWD) for connecting to the network.                                                          | MUST   | YES       |

## EMBRQ#01 — Proof of HTTPS API Access from Devices

Both devices can access the API via `https://yanisdeplazes.loca.lt` using HTTPS requests.
They can fetch data using `GET` and upload sensor readings using `POST`.

---

### GET: Fetch Devices

**File:** `Client.cpp` — Line 165 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/sensors/Client.cpp?ref_type=heads#L165)

```cpp
String raw = httpGET("/api/device");
```

### GET: Latest Reading for Device

**File:** `Client.cpp` — Line 202 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/installation/Client.cpp?ref_type=heads#L202)

```cpp
String raw = httpGET("/api/views/LatestDeviceReadings?device_id=1");
```

### POST: Register Device

**File:** `Client.cpp` — Line 208 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/sensors/Client.cpp?ref_type=heads#L208)

```cpp
StaticJsonDocument<128> doc;
doc["key"] = mac;
doc["name"] = "Weather Station";

String payload;
serializeJson(doc, payload);
httpPOST("/api/device", payload, "application/json");
```

### POST: Send Reading + Sensor Data

**File:** `Client.cpp` — Line 258 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/sensors/Client.cpp?ref_type=heads#L258)

```cpp
StaticJsonDocument<1024> doc;
doc["device_id"] = deviceId;
JsonArray arr = doc.createNestedArray("sensor_data");

JsonObject obj = arr.createNestedObject();
obj["sensor_id"] = 1;
obj["value"] = 24.5;

String payload;
serializeJson(doc, payload);
httpPOST("/api/reading-with-sensordata", payload, "application/json");
```

### Used Libraries

- `WiFiClientSecure` for HTTPS
- `ArduinoJson` for JSON handling

All traffic to and from the device is encrypted and uses HTTPS.

## EMBRQ#02

The ESP32 device hosts a HTTPS server using a self-signed certificate.
It exposes the `/index` route to get the current index of what device is currently displayed for a fallback.

### GET: /index Endpoint

**File:** `server.cpp` — Line ~45 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/installation/Server.cpp?ref_type=heads#L45)

```cpp
esp_err_t handle_index(httpd_req_t *req) {
    char resp_str[32];
    snprintf(resp_str, sizeof(resp_str), "{\"index\": %d}", currentIndex);

    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
}
```

### HTTPS Server Setup

**File:** `server.cpp` — Line ~63 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/installation/Server.cpp?ref_type=heads#L63)

```cpp
httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();
conf.servercert = (const uint8_t *)server_cert;
conf.servercert_len = strlen(server_cert) + 1;
conf.prvtkey_pem = (const uint8_t *)server_key;
conf.prvtkey_len = strlen(server_key) + 1;

esp_err_t ret = httpd_ssl_start(&server, &conf);
```

### Used Libraries

- `httpd_ssl` from ESP-IDF for TLS web server
- `esp_tls` for certificate handling
- Custom `cert.h` and `key.h` headers for in-memory PEM cert and key

### NGINX Settings

The system uses NGINX's Push Stream Module to broadcast the current index when it changes.
The subscribed client (IPad or Browser) can receive real-time updates over the WebSocket-like connection.

**File:** `nginx.conf` — Line ~51 [Source](https://github.com/YanisDeplazes/atmos/tree/main/docker/config/nginx/nginx.conf#L51)

```cpp
    location ^~ /api/installation/ {
            rewrite ^/api/installation(/.*)$ $1 break;
            proxy_pass https://100.74.255.21; // Hardcoded: not optimal
            proxy_ssl_verify off;
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_set_header X-Forwarded-Proto $scheme;
        }
```

## EMBRQ#03

## EMBRQ#03 — Sensor Data Collection

The reader device integrates 4 types of sensors (DHT11, BMP180, BH1750, analog water sensor) to measure environmental data and upload it to the backend API via HTTPS. Each sensor is initialized and polled in the main loop, with results printed to the serial monitor and sent as a single payload to the API.

### Loop

**File:** `sensors.ino` — Lines ~119 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/sensors/sensors.ino?ref_type=heads#L119)

```cpp
void loop() {
  readDht11();
  readBh1750();
  readWaterSensor();
  readBmp180();
  // ...
}
```

### Reading Functions

**File:** `sensors.ino` — Lines ~134 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/sensors/sensors.ino?ref_type=heads#L134)

```cpp
void readDht11() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  if (!isnan(temp)) sensorData[0].value = temp;
  if (!isnan(hum)) sensorData[1].value = hum;
}

void readBh1750() {
  float lux = lightMeter.readLightLevel();
  if (lux >= MIN_VALID_LUX) {
    sensorData[IDX_BH1750_LUX].value = lux;
  }
}

void readWaterSensor() {
  sensorData[IDX_WATER_ANALOG].value = analogRead(WATERPIN);
}

void readBmp180() {
  if (!bmpReady) return;

  sensorData[IDX_BMP180_TEMP].value = bmp.readTemperature();
  sensorData[IDX_BMP180_PRESSURE].value = bmp.readPressure() / PA_TO_HPA_DIVISOR;
}
```

### Used Libraries

- `DHT` for temperature and humidity (DHT11)
- `Adafruit_BMP085` for temperature and pressure (BMP180)
- `BH1750` for ambient light (lux)
- `Wire` (I2C) for communication

## EMBRQ#04

Both devices have output components (3 in total). They allow the for visually or audibly feedback from weather data.

- LED (Visualize when Setup is done)
- WS2812B Ledstrip (Visualize Temperature)
- Mini MP3 DFPlayer Player (Plays sound depending on weather)

### LED

**File:** `sensors.ino` — Lines ~111 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/sensors/sensors.ino?ref_type=heads#L111)

```cpp
  digitalWrite(WIFI_LED_PIN, HIGH);
```

### WS2812B Ledstrip

**File:** `LEDManager.cpp` — Lines ~50 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/installation/LEDManager.cpp?ref_type=heads#L50)

```cpp
void initLEDs() {
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(LED_BRIGHTNESS);
}
```

**File:** `LEDManager.cpp` — Lines ~86 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/installation/LEDManager.cpp?ref_type=heads#L86)

```cpp
void showColor(float temp) {
  CRGB color = (temp >= TEMP_COLOR_THRESHOLD)
                 ? interpolateColor(temp, TEMP_COLOR_THRESHOLD, TEMP_COLOR_MAX, COLOR_WARM_MIN, COLOR_WARM_MAX)
                 : interpolateColor(temp, TEMP_COLOR_MIN, TEMP_COLOR_THRESHOLD, COLOR_COLD_MIN, COLOR_COLD_MAX);

  for (int i = LOOP_START_INDEX; i < NUM_LEDS; i++) {
    leds[i] = color;
  }

  FastLED.show();

  Serial.printf("Static Temp Color → R: %d, G: %d, B: %d\n", color.r, color.g, color.b);
}
```

### Mini MP3 DFPlayer Player

**File:** `DFPlayerManager.cpp` — Lines ~60 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/installation/DFPlayerManager.cpp#L60)

```cpp
void initDFPlayer(HardwareSerial* serial, int rx, int tx) {
  dfSerial = serial;
  rxPin = rx;
  txPin = tx;

  dfSerial->begin(DFPLAYER_BAUDRATE, SERIAL_8N1, rxPin, txPin);

  if (!player.begin(*dfSerial)) {
    Serial.println("DFPlayer init failed");
    while (true); // Halt if initialization fails
  }

  setVolume(currentVolume);
}
```

**File:** `DFPlayerManager.cpp` — Lines ~L117 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/installation/DFPlayerManager.cpp#L117)

```cpp
void playTrack(uint8_t track) {
  if (track != lastTrackPlayed) {
    Serial.println("Playing new Track");
    Serial.println(track);
    player.loop(track);
    lastTrackPlayed = track;
  }
}
```

### Libraries Used

- DFRobotDFPlayerMini
- FastLED

## EMBRQ#05

Both devices use the WiFiManager library for the network setup.
If no credentials are stored, it launches a portal for configuration:

- No hardcoded credentials
- Portal fallback
- Auto-reconnect to known networks
- Reboots if config times out

**File:** installation: `WiFiSetup.cpp` — Lines ~21 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/sensors/WiFiSetup.cpp?ref_type=heads#L21)

**File:** sensors: `WiFiSetup.cpp` — Lines ~21 [Source](https://github.com/YanisDeplazes/atmos/tree/main/embedded/final/installation/WiFiSetup.cpp?ref_type=heads#L21)

```cpp
void connectToWiFi() {
  wifiManager.setConfigPortalTimeout(PORTAL_TIMEOUT_SEC);

  if (!wifiManager.autoConnect(WIFI_AP_NAME, WIFI_AP_PASSWORD)) {
    Serial.println("WiFi connection failed. Restarting...");
    delay(RESTART_DELAY_MS);
    ESP.restart();
  }

  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
```

### Libraries Used

- WiFiManager
