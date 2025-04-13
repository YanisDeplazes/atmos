# API Reference

## Base URL

```
http://YOURDOMAIN/api
```

For this project

```
https://yanisdeplazes.loca.lt/api
```

## Endpoints

### Data Tables Overview (CRUD)

| Table      | Endpoint      | Description                         |
| ---------- | ------------- | ----------------------------------- |
| Device     | `/device`     | Stores registered devices           |
| Reading    | `/reading`    | Stores sensor readings from devices |
| Sensor     | `/sensor`     | Defines available sensors           |
| SensorData | `/sensordata` | Links readings with sensor values   |
| Setting    | `/setting`    | Configuration settings              |
| Views      | `/views`      | Views                               |

### Custom Endpoints

| Endpoint                   | Description          |
| -------------------------- | -------------------- |
| `/reading-with-sensordata` | atomic POST request  |
| `/installation/publish`    | Publisher            |
| `/installation/ws`         | WebSocket connection |
| `/installation`            | reverse proxy        |

## Device

- `GET /device` - Get all devices
- `GET /device/{id}` - Get a device by ID
- `POST /device` - Create a device
- `PUT /device/{id}` - Update a device
- `DELETE /device/{id}` - Delete a device

### `GET /device`

**Example Response:**

```json
[
  {
    "id": "1",
    "key": "station_1",
    "name": "Weather Station 1"
  },
  {
    "id": "2",
    "key": "station_2",
    "name": "Weather Station 2"
  }
]
```

### `GET /device/{id}`

**Example Response:**

```json
{
  "id": 1,
  "key": "station_1",
  "name": "Weather Station 1"
}
```

### `POST /device`

**Example Request Body (Single Insert):**

```json
{
  "key": "station_3",
  "name": "Weather Station 3"
}
```

**Example Request Body (Multiple Inserts):**

```json
[
  { "key": "station_4", "name": "Weather Station 4" },
  { "key": "station_5", "name": "Weather Station 5" }
]
```

**Example Response:**

```json
{
  "message": "Created successfully"
}
```

### `PUT /device/{id}`

**Example Request Body:**

```json
{
  "name": "Updated Station"
}
```

**Example Response:**

```json
{
  "message": "Updated successfully"
}
```

### `DELETE /device/{id}`

**Example Response:**

```json
{
  "message": "Deleted successfully"
}
```

## Device

- `GET /reading` - Get all readings
- `GET /reading/{id}` - Get a reading by ID
- `POST /reading` - Create a reading
- `PUT /reading/{id}` - Update a reading
- `DELETE /reading/{id}` - Delete a reading

### `GET /reading`

**Example Response:**

```json
[
  {
    "id": "1",
    "device_id": "1",
    "timestamp": "2025-03-10 09:49:25"
  },
  {
    "id": "2",
    "device_id": "2",
    "timestamp": "2025-03-10 09:50:28"
  }
]
```

### `GET /reading/{id}`

**Example Response:**

```json
{
  "id": "1",
  "device_id": "1",
  "timestamp": "2025-03-10 09:49:25"
}
```

### `POST /reading`

**Example Request Body (Single Insert):**

```json
{
  "device_id": "1"
}
```

**Example Request Body (Multiple Inserts):**

```json
[{ "device_id": "1" }, { "device_id": "2" }]
```

**Example Response:**

```json
{
  "message": "Created successfully"
}
```

### `PUT /reading/{id}`

**Example Request Body:**

```json
{
  "device_id": "2"
}
```

**Example Response:**

```json
{
  "message": "Updated successfully"
}
```

### `DELETE /reading/{id}`

**Example Response:**

```json
{
  "message": "Deleted successfully"
}
```

## Sensor

- `GET /sensor` - Get all sensors
- `GET /sensor/{id}` - Get a sensor by ID
- `POST /sensor` - Create a sensor
- `PUT /sensor/{id}` - Update a sensor
- `DELETE /sensor/{id}` - Delete a sensor

### `GET /sensor`

**Example Response:**

```json
[
  {
    "id": "1",
    "key": "ky_015_temperature",
    "name": "KY-015 Temperature Sensor",
    "unit": "°C"
  },
  {
    "id": "2",
    "key": "ky_015_humidity",
    "name": "KY-015 Humidity Sensor",
    "unit": "%"
  },
  {
    "id": "3",
    "key": "gl5516_ldr",
    "name": "GL5516 LDR Photoresistor",
    "unit": "lux"
  },
  {
    "id": "4",
    "key": "dc3_5v_water",
    "name": "DC3-5V Water Sensor",
    "unit": "level"
  },
  {
    "id": "5",
    "key": "bmp280_temperature",
    "name": "BMP280 Temperature Sensor",
    "unit": "°C"
  },
  {
    "id": "6",
    "key": "bmp280_pressure",
    "name": "BMP280 Pressure Sensor",
    "unit": "hPa"
  }
]
```

### `GET /sensor/{id}`

**Example Response:**

```json
{
  "id": 1,
  "key": "ky_015_temperature",
  "name": "KY-015 Temperature Sensor",
  "unit": "°C"
}
```

### `POST /sensor`

**Example Request Body (Single Insert):**

```json
{
  "key": "ky_037_sound",
  "name": "KY-037 Microphone sound sensor (high sensitivity)",
  "unit": "dB"
}
```

**Example Request Body (Multiple Inserts):**

```json
[
  { "key": "sensor_1", "name": "Custom Sensor 1", "unit": "°C" },
  { "key": "sensor_2", "name": "Custom Sensor 2", "unit": "%" }
]
```

**Example Response:**

```json
{
  "message": "Created successfully"
}
```

### `PUT /sensor/{id}`

**Example Request Body:**

```json
{
  "name": "KY-037 Microphone sound sensor"
}
```

**Example Response:**

```json
{
  "message": "Updated successfully"
}
```

### `DELETE /sensor/{id}`

**Example Response:**

```json
{
  "message": "Deleted successfully"
}
```

## SensorData

- `GET /sensordata` - Get all sensor data
- `GET /sensordata/{id}` - Get a sensor data by ID
- `POST /sensordata` - Create a sensor data
- `PUT /sensordata/{id}` - Update a sensor data
- `DELETE /sensordata/{id}` - Delete a sensor data

### `GET /sensordata`

**Example Response:**

```json
[
  {
    "id": "1",
    "reading_id": "1",
    "sensor_id": "1",
    "value": "25.20"
  },
  {
    "id": "2",
    "reading_id": "1",
    "sensor_id": "3",
    "value": "150.00"
  },
  {
    "id": "3",
    "reading_id": "1",
    "sensor_id": "2",
    "value": "40.50"
  },
  {
    "id": "4",
    "reading_id": "1",
    "sensor_id": "4",
    "value": "1.00"
  },
  {
    "id": "5",
    "reading_id": "1",
    "sensor_id": "5",
    "value": "21.70"
  },
  {
    "id": "6",
    "reading_id": "1",
    "sensor_id": "6",
    "value": "1010.80"
  }
]
```

### `GET /sensordata/{id}`

**Example Response:**

```json
{
  "id": 1,
  "reading_id": 1,
  "sensor_id": 1,
  "value": "25.20"
}
```

### `POST /sensordata`

**Example Request Body (Single Insert):**

```json
{
  "reading_id": 2,
  "sensor_id": 1,
  "value": "26.20"
}
```

**Example Request Body (Multiple Inserts):**

```json
[
  { "reading_id": 2, "sensor_id": 1, "value": "26.20" },
  { "reading_id": 2, "sensor_id": 2, "value": "50.00" }
]
```

**Example Response:**

```json
{
  "message": "Created successfully"
}
```

### `PUT /sensordata/{id}`

**Example Request Body:**

```json
{
  "value": "27.20"
}
```

**Example Response:**

```json
{
  "message": "Updated successfully"
}
```

### `DELETE /sensordata/{id}`

**Example Response:**

```json
{
  "message": "Deleted successfully"
}
```

## Setting

- `GET /setting` - Get all settings
- `GET /setting/{id}` - Get a setting by ID
- `POST /setting` - Create a setting
- `PUT /setting/{id}` - Update a setting
- `DELETE /setting/{id}` - Delete a setting

### `GET /setting`

**Example Response:**

```json
[
  {
    "id": "1",
    "key": "pressure_min",
    "name": "Minimum Atmospheric Pressure",
    "value": null,
    "default_value": "900.0000"
  },
  {
    "id": "2",
    "key": "pressure_max",
    "name": "Maximum Atmospheric Pressure",
    "value": null,
    "default_value": "1080.0000"
  },
  {
    "id": "3",
    "key": "wind_speed_min",
    "name": "Minimum Wind Speed",
    "value": null,
    "default_value": "0.0000"
  },
  {
    "id": "4",
    "key": "wind_speed_max",
    "name": "Maximum Wind Speed",
    "value": null,
    "default_value": "100.0000"
  },
  {
    "id": "5",
    "key": "temperature_min",
    "name": "Minimum Temperature",
    "value": null,
    "default_value": "-10.0000"
  },
  {
    "id": "6",
    "key": "temperature_max",
    "name": "Maximum Temperature",
    "value": null,
    "default_value": "50.0000"
  },
  {
    "id": "7",
    "key": "lux_min",
    "name": "Minimum Light Intensity (Lux)",
    "value": null,
    "default_value": "0.0000"
  },
  {
    "id": "8",
    "key": "lux_max",
    "name": "Maximum Light Intensity (Lux)",
    "value": null,
    "default_value": "1000.0000"
  },
  {
    "id": "9",
    "key": "blob_intensity_min",
    "name": "Minimum Blob Intensity",
    "value": null,
    "default_value": "0.0500"
  },
  {
    "id": "10",
    "key": "blob_intensity_max",
    "name": "Maximum Blob Intensity",
    "value": null,
    "default_value": "1.0000"
  },
  {
    "id": "11",
    "key": "blob_speed_min",
    "name": "Minimum Blob Movement Speed",
    "value": null,
    "default_value": "0.0100"
  },
  {
    "id": "12",
    "key": "blob_speed_max",
    "name": "Maximum Blob Movement Speed",
    "value": null,
    "default_value": "0.5000"
  },
  {
    "id": "13",
    "key": "background_noise_scale_min",
    "name": "Minimum Background Noise Scale",
    "value": null,
    "default_value": "5.0000"
  },
  {
    "id": "14",
    "key": "background_noise_scale_max",
    "name": "Maximum Background Noise Scale",
    "value": null,
    "default_value": "10.0000"
  },
  {
    "id": "15",
    "key": "background_height_min",
    "name": "Minimum Background Height",
    "value": null,
    "default_value": "0.5000"
  },
  {
    "id": "16",
    "key": "background_height_max",
    "name": "Maximum Background Height",
    "value": null,
    "default_value": "2.5000"
  },
  {
    "id": "17",
    "key": "background_speed_min",
    "name": "Minimum Background Movement Speed",
    "value": null,
    "default_value": "1.0000"
  },
  {
    "id": "18",
    "key": "background_speed_max",
    "name": "Maximum Background Movement Speed",
    "value": null,
    "default_value": "60.0000"
  },
  {
    "id": "19",
    "key": "background_brightness_min",
    "name": "Minimum Background Brightness",
    "value": null,
    "default_value": "0.1000"
  },
  {
    "id": "20",
    "key": "background_brightness_max",
    "name": "Maximum Background Brightness",
    "value": null,
    "default_value": "1.0000"
  },
  {
    "id": "21",
    "key": "color_temperature_threshold",
    "name": "Temperature Threshold for Color Changes",
    "value": null,
    "default_value": "10.0000"
  },
  {
    "id": "22",
    "key": "color_humidity_brightness_factor",
    "name": "Impact of Humidity on Color Brightness",
    "value": null,
    "default_value": "1.0000"
  }
]
```

### `GET /setting/{id}`

**Example Response:**

```json
{
  "id": 1,
  "key": "pressure_min",
  "name": "Minimum Atmospheric Pressure",
  "value": null,
  "default_value": "900.0000"
}
```

### `POST /setting`

**Example Request Body (Single Insert):**

```json
{
  "key": "xyz",
  "name": "XYZ Settings",
  "value": null,
  "default_value": "900.0000"
}
```

**Example Request Body (Multiple Inserts):**

```json
[
  {
    "key": "setting_1",
    "name": "Setting 1",
    "value": "50",
    "default_value": "100"
  },
  {
    "key": "setting_2",
    "name": "Setting 2",
    "value": "25",
    "default_value": "80"
  }
]
```

**Example Response:**

```json
{
  "message": "Created successfully"
}
```

### `PUT /setting/{id}`

**Example Request Body:**

```json
{
  "value": 31.1
}
```

**Example Response:**

```json
{
  "message": "Updated successfully"
}
```

### `DELETE /setting/{id}`

**Example Response:**

```json
{
  "message": "Deleted successfully"
}
```

## Views

### `get /views/LatestDeviceReadings?device_id={id}`

This view retrieves only the **most recent** sensor readings for each device.

**Example Response:**

```json
[
  {
    "device_id": "50",
    "device_name": "Weather Station 3",
    "sensor_name": "KY-015 Temperature Sensor",
    "sensor_unit": "°C",
    "value": "1.20",
    "timestamp": "2025-03-10 18:09:40"
  },
  {
    "device_id": "50",
    "device_name": "Weather Station 3",
    "sensor_name": "KY-015 Humidity Sensor",
    "sensor_unit": "%",
    "value": "10.50",
    "timestamp": "2025-03-10 18:09:40"
  },
  {
    "device_id": 1,
    "device_name": "Weather Station 1",
    "sensor_name": "KY-015 Temperature Sensor",
    "sensor_unit": "°C",
    "value": "25.20",
    "timestamp": "2025-03-10 09:49:25"
  },
  {
    "device_id": 1,
    "device_name": "Weather Station 1",
    "sensor_name": "GL5516 LDR Photoresistor",
    "sensor_unit": "lux",
    "value": "150.00",
    "timestamp": "2025-03-10 09:49:25"
  }
  // ...
]
```

## Custom API Requests

### `POST /reading-with-sensordata`

Creates a reading and its associated sensor data in a single atomic POST request.

**Example Request Body:**

```json
{
  "device_id": 1,
  "sensor_data": [
    { "sensor_id": 1, "value": 19.8 },
    { "sensor_id": 2, "value": 10.0 },
    { "sensor_id": 3, "value": 0.0 },
    { "sensor_id": 4, "value": 51.0 },
    { "sensor_id": 5, "value": 20.2 },
    { "sensor_id": 6, "value": 1027.8 }
  ]
}
```

**Example Response:**

```json
{
  "message": "Created successfully"
}
```

### `POST /reading-with-sensordata`

## Installation WebSocket & Publishing Endpoints

These endpoints are used for **real-time communication** between the backend and frontend via **WebSocket-over-TLS**, powered by `ngx_http_push_stream_module`.

## WebSocket Subscriber

### `GET /installation/ws`

Establishes a **WebSocket connection** and subscribes the client to the `installation` channel.

- **Protocol:** WebSocket (`wss://` in production)
- **Expected Use Case:** Frontend client connects here to receive real-time updates (e.g., active carousel index).
- **Connection Type:** Long-lived, bidirectional.

## Publisher Endpoint

### `POST /installation/publish`

Publishes a new message to all subscribers of the `installation` channel.

- **Body Format (Raw text):**

```text
1
```

- **Content-Type:** `text/plain`
- **Authentication:** Restricted (handled by server access control or internal-only publishing)
- **Expected Use Case:** The backend (or ESP32) sends state updates like `currentIndex` to the frontend via this channel.

## Installation Reverse Proxy Endpoint

### `GET /installation/...`

All remaining requests starting with `/installation/` are **reverse proxied** to the embedded HTTPS server of the ESP32:

```nginx
proxy_pass https://100.74.255.21; // Should be optimized. Maybe use Local Domains
```

- **Use Case:** Internal API call delegation to ESP32-secured HTTPS endpoints.
- **Security:** Certificate verification is skipped (`proxy_ssl_verify off`),
