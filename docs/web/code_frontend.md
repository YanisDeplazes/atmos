# Code: Frontend

## Disclaimer

In this part, ChatGPT was used to generate code comments such as class, method and function headers, which helped to maintain a standard and write correct code comments. It is a reflection of the code comments and structure of the code.

## App Class (`main.js`)

**Main entry point** for the application. Initializes the **background animation, IoT devices, and UI components**. Uses the **ElementRegistry** for efficient UI management.

### Properties

| Name              | Type              | Visibility | Description                          |
| ----------------- | ----------------- | ---------- | ------------------------------------ |
| `backgroundScene` | `BackgroundScene` | private    | The background scene instance.method |
| `devices`         | `Device[]`        | private    | An array storing all IoT devices.    |

### Methods

#### `constructor()`

Initializes the application by **registering UI elements** and setting up core components.

- **Visibility:** Public

#### `init()`

Main initialization method.

- **Initializes the Three.js background**
- **Loads IoT devices asynchronously**
- **Initializes UI components**

- **Visibility:** Public
- **Return Type:** `Promise<void>`

#### `initBackground()`

Initializes the **Three.js animated background**.

- **Visibility:** Public

#### `initDevices()`

Fetches and initializes all **available IoT devices**.

- **Visibility:** Public
- **Return Type:** `Promise<void>`

#### `registerUIElements()`

**Registers key UI elements** into the `ElementRegistry` for efficient **DOM access and management**.

- **Visibility:** Public

#### `initUI()`

**Initializes UI components** such as:

- `Loader` → Handles app startup animation
- `Select` → Manages the selection menu
- `Carousel` → Enables navigation through devices

- **Visibility:** Public

## ApiClient Helpers Class (`helpers/ApiClient.js`)

Handles communication with the backend API to fetch and update data dynamically. Provides methods for retrieving sensor readings, device lists, and application settings.

### Properties

| Name           | Type     | Visibility | Description                                                               |
| -------------- | -------- | ---------- | ------------------------------------------------------------------------- |
| `API_BASE_URL` | `string` | public     | Base URL for API requests, dynamically set based on the current hostname. |

### Methods

#### `fetchCurrentIndex()`

Retrieves the current index from the installation endpoint. (ESP32)

- **Visibility:** Public
- **Return Type:** `Promise<object[] | null>`

#### `fetchLatestDeviceReadings(deviceId)`

Fetches the latest sensor data for a specific device.

- **Visibility:** Public
- **Parameter:** `deviceId`: `string` The unique identifier of the device.
- **Return Type:** `Promise<object[] | null>`

#### `fetchDevices()`

Fetches the list of available devices.

- **Visibility:** Public
- **Return Type:** `Promise<object[]>`

#### `updateSetting(id, value)`

Updates a specific setting in the database.

- **Visibility:** Public
- **Parameter:** `id`: `string` The unique identifier of the setting.
- **Parameter:** `value`: `number|string` The new value for the setting.
- **Return Type:** `Promise<boolean>`

#### `getSettings()`

Retrieves application settings from the database.

- **Visibility:** Public
- **Return Type:** `Promise<object>`

## WebSocketClient Helpers Class (`helpers/WebSocketClient.js`)

Handles real-time communication with the backend using WebSocket-over-TLS. Synchronizes the active device index across connected clients and updates the frontend carousel in real-time.

### Properties

| Name       | Type                | Visibility | Description                                                                  |
| ---------- | ------------------- | ---------- | ---------------------------------------------------------------------------- |
| `socket`   | `WebSocket \| null` | static     | Holds the active WebSocket connection instance or `null` if not connected.   |
| `carousel` | `Carousel \| null`  | static     | Reference to the carousel instance used for updating the UI.                 |
| `URL`      | `string`            | static     | WebSocket endpoint dynamically built from the current hostname and protocol. |

### Methods

#### `setup(carousel)`

Registers a `Carousel` instance to be controlled by the WebSocket client.

- **Visibility:** Public
- **Parameter:** `carousel`: `Carousel` — The carousel instance to register.

#### `connect()`

Establishes a secure WebSocket connection to the backend, listens for index updates, and synchronizes the UI.

- **Visibility:** Public
- **Return Type:** `Promise<void>`

#### `disconnect()`

Closes the existing WebSocket connection if active and cleans up resources.

- **Visibility:** Public

## ElementRegistry Helpers Class (`helpers/ElementRegistry.js`)

Manages global references to UI elements. Provides an efficient way to register, retrieve, and manipulate DOM elements without repeated queries.

### Properties

| Name       | Type                       | Visibility | Description                               |
| ---------- | -------------------------- | ---------- | ----------------------------------------- |
| `elements` | `Map<string, HTMLElement>` | public     | A Map storing registered elements by key. |

### Methods

#### `register(key, element)`

Registers an HTML element for easy access.

- **Visibility:** Public
- **Parameter:** `key`: `string` Unique identifier for the element.
- **Parameter:** `element`: `HTMLElement` The DOM element to register.

#### `get(key)`

Retrieves a previously registered element.

- **Visibility:** Public
- **Parameter:** `key`: `string` The identifier of the registered element.
- **Return Type:** `HTMLElement|null`

#### `get(key)`

Retrieves a previously registered element.

- **Visibility:** Public
- **Parameter:** `key`: `string` The identifier of the registered element.
- **Return Type:** `HTMLElement|null`

## Settings Helpers Class (`helpers/Settings.js`)

Manages application settings by dynamically fetching and updating configuration values from the API. Converts setting keys from snake_case to camelCase for easier access in JavaScript. Also dynamically updates the settings UI and allows saving changes back to the database.

### Properties

| Name       | Type                      | Visibility | Description                                                                                         |
| ---------- | ------------------------- | ---------- | --------------------------------------------------------------------------------------------------- |
| `settings` | `Record<string, Setting>` | public     | Stores dynamically loaded settings from the API. Keys are formatted in camelCase for easier access. |

### Methods

#### `loadSettings()`

Fetches settings from the API and dynamically stores them. Updates the settings UI with input fields allowing user modifications.

- **Visibility:** Public
- **Return Type:** `Promise<void>`

#### `submitSettings()`

Sends updated settings to the API to be stored in the database.

- **Visibility:** Public
- **Return Type:** `Promise<void>`

#### `get(key)`

Retrieves a setting value from the loaded settings.

- **Visibility:** Public
- **Parameter:** `key`: `string` The setting key in camelCase format.
- **Return Type:** `number|null`

## Utils Helpers Class (`helpers/Utils.js`)

Utility class providing helper functions for value clamping, color interpolation based on temperature, humidity-based adjustments, and DOM element visibility manipulation.

### Methods

#### `clamp(value, min, max)`

Clamps a value within a specified range.

- **Visibility:** Public
- **Parameter:** `value`: `number` The value to be clamped.
- **Parameter:** `min`: `number` The minimum allowed value.
- **Parameter:** `max`: `number` The maximum allowed value.
- **Return Type:** `number`

#### `getPrimaryColorr(temperature)`

Determines the primary color based on temperature using smooth interpolation.

- **Visibility:** Public
- **Parameter:** `temperature`: `number` The temperature value.
- **Return Type:** `string`

#### `interpolateColor(value, stops)`

Interpolates between color stops based on a given value.

- **Visibility:** Public
- **Parameter:** `temperature`: `number` The input value (e.g., temperature).
- **Parameter:** `temperature`: `ColorStop[]` Array of color stops with temperature and RGB values.
- **Return Type:** `string`

#### `getSecondaryColor(temperature, humidity)`

Computes a secondary color based on temperature and humidity. The brightness is adjusted by blending humidity influence into the primary color.

- **Visibility:** Public
- **Parameter:** `temperature`: `number` The temperature value.
- **Parameter:** `humidity`: `number` Humidity percentage (0-100).
- **Return Type:** `string`

#### `hide(element)`

Hides a given DOM element by adding a "hidden" CSS class.

- **Visibility:** Public
- **Parameter:** `element`: `HTMLElement` The DOM element to hide.

#### `show(element)`

Shows a given DOM element by removing the "hidden" CSS class.

- **Visibility:** Public
- **Parameter:** `element`: `HTMLElement` The DOM element to show.

#### `toggle(element)`

Toggles the visibility of a registered element.

- **Visibility:** Public
- **Parameter:** `element`: `HTMLElement` The DOM element to toggle.

#### `getRandomValue(min, max)`

Generates a random value within the given range.

- **Visibility:** Public
- **Parameter:** `min`: `number` The minimum value.
- **Parameter:** `max`: `number` The maximum value.
- **Return Type:** `number`

## BackgroundScene Class (`components/BackgroundScene.js`)

Singleton class representing an animated **Three.js background**.Dynamically adjusts based on environmental sensor data such as **wind speed, pressure, temperature, humidity, and brightness**.

### Properties

| **Name**           | **Type**                  | **Visibility** | **Description**                                         |
| ------------------ | ------------------------- | -------------- | ------------------------------------------------------- |
| `container`        | `HTMLElement`             | `public`       | The HTML container element where the scene is rendered. |
| `scene`            | `THREE.Scene`             | `public`       | The Three.js scene.                                     |
| `camera`           | `THREE.PerspectiveCamera` | `public`       | The Three.js camera.                                    |
| `renderer`         | `THREE.WebGLRenderer`     | `public`       | The WebGL renderer for the scene.                       |
| `plane`            | `THREE.Mesh`              | `public`       | The animated background mesh.                           |
| `uniforms`         | `object`                  | `public`       | Shader uniforms controlling animation behavior.         |
| `targetHeight`     | `number`                  | `public`       | Target Height for smooth transitions.                   |
| `targetNoiseScale` | `number`                  | `public`       | Target Noise Scale for smooth transitions.              |
| `targetSpeed`      | `number`                  | `public`       | Target Speed for smooth transitions.                    |
| `targetColor1`     | `number`                  | `public`       | Target Primary Color for smooth transitions.            |
| `targetColor2`     | `number`                  | `public`       | Target Secondary Color High for smooth transitions.     |
| `targetBrightness` | `number`                  | `public`       | Target Brightness for smooth transitions.               |
| `lerpSpeed`        | `number`                  | `public`       | Controls the smoothness of transitions.                 |

### Methods

#### `constructor(containerId)`

Creates or returns the existing BackgroundScene instance.

- **Visibility:** Public
- **Parameter:** `containerId`: `string` The ID of the HTML container where the scene will be rendered.

#### `initScene()`

Initializes the Three.js scene.

- **Visibility:** Public

#### `initCamera()`

Initializes the camera with a fixed **perspective view**.

- **Visibility:** Public

#### `initRenderer()`

Initializes the WebGL renderer.

- **Visibility:** Public

#### `initBackground()`

Creates and adds the **animated background plane**.

- **Visibility:** Public

#### `resize()`

Handles resizing to maintain **correct aspect ratio**.

- **Visibility:** Public

#### `animate()`

**Animation Loop**

- Updates **shader time**
- Smoothly **transitions between target values**
- Renders the scene on each frame

- **Visibility:** Public

#### `updateAppearance(pressure, temperature, humidity, light)`

**Updates the Background Appearance** based on real-time sensor data.

- **Visibility:** Public
- **Parameter:** `pressure`: `number` Atmospheric pressure in hPa.
- **Parameter:** `temperature`: `number` Temperature in °C.
- **Parameter:** `humidity`: `number` Relative humidity in %.
- **Parameter:** `light`: `number` Ambient light in lux.

#### `setNoiseScale(pressure)`

Updates the background noise scale based on atmospheric pressure.
Lower pressure results in more turbulent, higher noise scale.

- **Visibility:** Public
- **Parameter:** `pressure`: `number` Atmospheric pressure in hPa.

#### `setHeight(pressure)`

Adjusts the background vertical height based on atmospheric pressure.
Lower pressure raises the background to simulate uplift.

- **Visibility:** Public
- **Parameter:** `pressure`: `number` Atmospheric pressure in hPa.

#### `setSpeed(pressure)`

Adjusts background animation speed based on atmospheric pressure.
Lower pressure increases animation speed to simulate stronger winds.

- **Visibility:** Public
- **Parameter:** `windSpeed`: `number` Atmospheric pressure in hPa.

#### `setColor1(temperature)`

Updates the cooler base color of the background based on temperature.

- **Visibility:** Public
- **Parameter:** `temperature`: `number` Temperature in °C.

#### `setColor2(temperature, humidity)`

Updates the warmer highlight color of the background using temperature and humidity.

- **Visibility:** Public
- **Parameter:** `temperature`: `number` Temperature in °C.
- **Parameter:** `humidity`: `number` Relative humidity in %.

#### `setBrightness(lux)`

Adjusts scene brightness based on ambient light in lux.
Lux is mapped to a normalized brightness range..

- **Visibility:** Public
- **Parameter:** `lux`: `number` Ambient light level in lux.

#### `getInstance(containerId = "backgroundContainer")`

**Static method** to get the **singleton instance**.

- **Visibility:** Public
- **Parameter:** `containerId`: `string` The container ID (only needed for first initialization).
- **Return Type:** `BackgroundScene`

## BlobScene Class (`components/BlobScene.js`)

Represents a **3D animated Blob** using Three.js. The blob dynamically changes **intensity, speed, and colors** based on real-time **sensor data**.

### Properties

| **Name**          | **Type**                  | **Visibility** | **Description**                                             |
| ----------------- | ------------------------- | -------------- | ----------------------------------------------------------- |
| `container`       | `HTMLElement`             | `public`       | The container element where the Three.js scene is rendered. |
| `scene`           | `THREE.Scene`             | `public`       | The Three.js scene.                                         |
| `camera`          | `THREE.PerspectiveCamera` | `public`       | The Three.js camera.                                        |
| `renderer`        | `THREE.WebGLRenderer`     | `public`       | The WebGL renderer for the scene.                           |
| `controls`        | `OrbitControls`           | `public`       | The orbit controls for camera interaction.                  |
| `blob`            | `THREE.Mesh`              | `public`       | The animated blob mesh.                                     |
| `uniforms`        | `object`                  | `public`       | Shader uniforms controlling animation behavior.             |
| `targetIntensity` | `number`                  | `public`       | Target intensity for smooth transitions.                    |
| `targetSpeed`     | `number`                  | `public`       | Target speed for smooth transitions.                        |
| `targetColor1`    | `number`                  | `public`       | Target primary color for smooth transitions.                |
| `targetColor2`    | `number`                  | `public`       | Target secondary color for smooth transitions.              |
| `lerpSpeed`       | `number`                  | `public`       | Controls the smoothness of transitions.                     |

### Methods

#### `constructor(containerId)`

Creates a new **BlobScene** instance.

- **Visibility:** Public
- **Parameter:** `containerId`: `string` The **ID** of the HTML container where the scene will be rendered.

#### `initScene()`

Initializes the **Three.js Scene**.

- **Visibility:** Public

#### `initCamera()`

Initializes the **Camera** with a fixed **1:1 aspect ratio**.

- **Visibility:** Public

#### `initRenderer()`

Initializes the **WebGL Renderer**.

- **Visibility:** Public

#### `initControls()`

Initializes **Orbit Controls** for user interaction.

- **Visibility:** Public

#### `initBlob()`

Creates and adds the **animated blob mesh** to the scene.

- **Visibility:** Public

#### `resize()`

Handles **resizing** to maintain a **1:1 aspect ratio**.

- **Visibility:** Public

#### `animate()`

**Animation Loop**

- Updates **shader time**
- Smoothly **transitions between target values**
- Renders the scene on each frame

- **Visibility:** Public

#### `updateAppearance(pressure, temperature, humidity)`

**Updates the Blob Appearance** based on real-time sensor data.

- **Visibility:** Public
- **Parameter:** `pressure`: `number` Current atmospheric pressure (hPa).
- **Parameter:** `temperature`: `number` Current temperature (°C).
- **Parameter:** `humidity`: `number` Current relative humidity (%).

#### `setIntensity(pressure)`

Adjusts blob intensity based on atmospheric pressure.
Lower pressure → higher intensity (chaotic visuals),
Higher pressure → lower intensity (calm visuals).

- **Visibility:** Public
- **Parameter:** `pressure`: `number` Atmospheric pressure in hPa.

#### `setSpeed(pressure)`

Adjusts blob animation speed based on atmospheric pressure.
Lower pressure → faster motion (energetic),
Higher pressure → slower motion (calmer).

- **Visibility:** Public
- **Parameter:** `pressure`: `number` Atmospheric pressure in hPa.

#### `setColor1(temperature)`

Updates the primary blob color based on temperature.
Warmer or cooler tones reflect the temperature visually.

- **Visibility:** Public
- **Parameter:** `temperature`: `number` Temperature in °C.

#### `setColor2(temperature, humidity)`

Updates the secondary blob color based on both temperature and humidity.
Adds complexity to the color blending based on conditions.

- **Visibility:** Public
- **Parameter:** `temperature`: `number` Temperature in °C.
- **Parameter:** `humidity`: `number` Relative humidity in %.

## Carousel Class (`components/Carousel.js`)

Manages a dynamically interactive image carousel with navigation dots. Enables users to cycle through carousel items and updates the active device for sensor data visualization.

### Properties

| **Name**        | **Type**           | **Visibility** | **Description**                                  |
| --------------- | ------------------ | -------------- | ------------------------------------------------ |
| `carousel`      | `HTMLElement/null` | `private`      | The carousel container element.                  |
| `dotsContainer` | `HTMLElement/null` | `private`      | The container holding navigation dots.           |
| `carouselItems` | `HTMLElement[]`    | `private`      | An array of all carousel items.                  |
| `dots`          | `HTMLElement[]`    | `private`      | An array of all navigation dots.                 |
| `activeIndex`   | `number`           | `private`      | The index of the currently active carousel item. |

### Methods

#### `constructor(carouselSelector, dotsContainerSelector)`

Creates a new Carousel instance.

- **Visibility:** Public
- **Parameter:** `carouselSelector`: `string` CSS selector for the carousel container.
- **Parameter:** `dotsContainerSelector`: `string` CSS selector for the navigation dots container.

#### `init()`

Initializes the carousel by setting up event listeners and updating UI.

- **Visibility:** Public

#### `createDotsList()`

Generates the navigation dots dynamically based on the number of carousel items.

- **Visibility:** Public

#### `updateCarousel()`

Updates the carousel to reflect the currently active item. Adjusts positioning and triggers UI transitions.

- **Visibility:** Public

#### `updateDots()`

Updates the navigation dots to indicate the currently active carousel item.

- **Visibility:** Public

#### `shiftLeft()`

Moves the carousel to the previous item.

- **Visibility:** Public

#### `shiftRight()`

Moves the carousel to the next item.

- **Visibility:** Public

#### `addEventListeners()`

**Animation Loop**

Adds event listeners for user interactions with the carousel and navigation dots.

- **Visibility:** Public

## Device Class (`components/Device.js`)

Represents an IoT device, handling sensor data retrieval, UI updates, and real-time visualizations through blob animations.

### Properties

| **Name**          | **Type**           | **Visibility** | **Description**                                                |
| ----------------- | ------------------ | -------------- | -------------------------------------------------------------- |
| `id`              | `string`           | `private`      | The unique identifier of the device.                           |
| `key`             | `string`           | `private`      | The device key used for identifying the IoT station.           |
| `name`            | `string`           | `private`      | The name of the device.                                        |
| `sensorData`      | `object[]`         | `private`      | Stores the latest sensor data.                                 |
| `isActive`        | `boolean`          | `private`      | Tracks whether the device is currently active in the carousel. |
| `updateInterval`  | `number/null`      | `private`      | Interval ID for periodic sensor updates.                       |
| `backgroundScene` | `BackgroundScene`  | `private`      | Instance of the background scene.                              |
| `blobCanvasId`    | `string`           | `private`      | The ID of the canvas element for blob animations.              |
| `carouselItem`    | `HTMLElement/null` | `private`      | The carousel item representing the device.                     |
| `blobScene`       | `BlobScene`        | `private`      | Instance of the blob scene visualization.                      |

### Methods

#### `  constructor({ id, key, name })`

Creates a new IoT Device instance.

- **Visibility:** Public
- **Parameter:** `config`: `object` Device configuration.
- **Parameter:** `config.id`: `string` Unique device identifier.
- **Parameter:** `config.key`: `string` Device key (e.g., "station_1").
- **Parameter:** `config.name`: `string` Device name (e.g., "Weather Station 1").

#### `updateSensorData()`

Fetches and updates the device's sensor data. Updates both the blob animation and UI elements if the device is active.

- **Visibility:** Public
- **Return Type:** `Promise<void>`

#### `getSensorValue(sensorType)`

Extracts a specific sensor value from the sensor data array.

- **Visibility:** Public
- **Parameter:** `sensorType`: `string` The sensor type to retrieve.
- **Return Type:** `number|undefined`

#### `createCarouselItem()`

Creates a carousel item representing the device..

- **Visibility:** Public
- **Return Type:** `HTMLElement|null`

#### `attachEventListeners()`

Attaches event listeners to handle device activation within the carousel.

- **Visibility:** Public

#### `generateMeasurementList()`

Generates the sensor measurement list for UI updates.

- **Visibility:** Public
- **Return Type:** `string`

#### `refreshUI()`

Updates the UI with the latest sensor data.

- **Visibility:** Public

#### `startUpdating()`

Starts fetching sensor data at regular intervals.

- **Visibility:** Public
- **Return Type:** `Promise<void>`

#### `stopUpdating()`

Stops the periodic fetching of sensor data.

- **Visibility:** Public

## Loader Class (`components/Loader.js`)

Manages the application's loading animation and prepares the environment Uses a dynamic blob animation during the loading phase and transitions into the main UI after a specified duration.

### Properties

| **Name**          | **Type**           | **Visibility** | **Description**                                                 |
| ----------------- | ------------------ | -------------- | --------------------------------------------------------------- |
| `loader`          | `HTMLElement/null` | `private`      | The loader element.                                             |
| `blobScene`       | `HTMLElement/null` | `private`      | Instance of the BlobScene, responsible for animated visuals.    |
| `backgroundScene` | `BackgroundScene`  | `private`      | Background scene instance.                                      |
| `updateInterval`  | `number/null`      | `private`      | Interval ID for periodic updates to the blob animation.         |
| `displayDuration` | `number`           | `private`      | Duration (in milliseconds) before transitioning to the main UI. |
| `updateFrequency` | `number`           | `private`      | Frequency (in milliseconds) of blob updates.                    |
| `callback`        | `Function`         | `private`      | Callback function executed after loading is complete.           |

### Methods

#### `  constructor(callback)`

Creates a new Loader instance.

- **Visibility:** Public
- **Parameter:** `callback`: `Function` Function to execute after loading completes..

#### `init()`

Initializes the loader by creating the UI, starting the animation, and scheduling the transition to the main UI.

- **Visibility:** Public

#### `createHTML()`

Creates the loading screen UI elements. Adds a blob animation and heading text.

- **Visibility:** Public

#### `startUpdatingBlob()`

Starts periodically updating the blob's appearance based on simulated environment data.

- **Visibility:** Public

#### `updateBlobAppearance()`

Updates the blob and background animation using randomly generated values that mimic environmental conditions.

- **Visibility:** Public

#### `cleanup()`

Cleans up the loading animation by stopping updates and hiding the loader UI.

- **Visibility:** Public

#### `stopUpdatingBlob()`

Stops the periodic updates to the blob animation.

- **Visibility:** Public

#### `stopUpdatingBlob()`

Stops the periodic updates to the blob animation.

- **Visibility:** Public

## Select Class (`components/Select.js`)

Manages the application's loading animation and prepares the environment Uses a dynamic blob animation during the loading phase and transitions into the main UI after a specified duration.

### Properties

| **Name** | **Type**           | **Visibility** | **Description**             |
| -------- | ------------------ | -------------- | --------------------------- |
| `select` | `HTMLElement/null` | `private`      | The selection menu element. |

### Methods

#### `  constructor()`

Initializes the selection menu and retrieves necessary UI elements. If the select element is not found, logs an error.

- **Visibility:** Public

#### `init()`

Dynamically creates the selection menu with available options. Clears existing elements and appends new menu items.

- **Visibility:** Public

#### `createHTML()`

Handles the user selection and updates the UI accordingly.

- **Visibility:** Public

#### `handleSelection(option)`

Starts periodically updating the blob's appearance based on simulated environment data.

- **Visibility:** Public
- **Parameter:** `option`: `string` The selected menu option.

#### `showInstallation()`

Displays the installation view, updates UI elements, and triggers device updates. Hides the background and applies the "installation" class to the carousel.

- **Visibility:** Public

#### `showOverview()`

Displays the overview view and ensures all relevant UI elements are visible.

- **Visibility:** Public
