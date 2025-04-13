import BackgroundScene from "./BackgroundScene";
import BlobScene from "./BlobScene";
import ApiClient from "../helpers/ApiClient";
import ElementRegistry from "../helpers/ElementRegistry";

/**
 * Represents an IoT device, handling sensor data retrieval, UI updates,
 * and real-time visualizations through blob animations.
 *
 * @class Device
 * @author Yanis Deplazes
 * date 10.03.2025
 */
export default class Device {
  /**
   * The unique identifier of the device.
   * @type {string}
   * @private
   */
  id;

  /**
   * The device key used for identifying the IoT station.
   * @type {string}
   * @private
   */
  key;

  /**
   * The name of the device.
   * @type {string}
   * @private
   */
  name;

  /**
   * Stores the latest sensor data.
   * @type {object[]}
   * @private
   */
  sensorData = {};

  /**
   * Tracks whether the device is currently active in the carousel.
   * @type {boolean}
   * @private
   */
  isActive = false;

  /**
   * Interval ID for periodic sensor updates.
   * @type {number|null}
   * @private
   */
  updateInterval = null;

  /**
   * Instance of the background scene.
   * @type {BackgroundScene}
   * @private
   */
  backgroundScene;

  /**
   * The ID of the canvas element for blob animations.
   * @type {string}
   * @private
   */
  blobCanvasId;

  /**
   * The carousel item representing the device.
   * @type {HTMLElement|null}
   * @private
   */
  carouselItem;

  /**
   * Instance of the blob scene visualization.
   * @type {BlobScene}
   * @private
   */
  blobScene;

  /**
   * Creates a new IoT Device instance.
   *
   * @param {object} config - Device configuration.
   * @param {string} config.id - Unique device identifier.
   * @param {string} config.key - Device key (e.g., "station_1").
   * @param {string} config.name - Device name (e.g., "Weather Station 1").
   */
  constructor({ id, key, name }) {
    this.id = id;
    this.key = key;
    this.name = name;
    this.backgroundScene = BackgroundScene.getInstance();
    this.blobCanvasId = `blobCanvas-${this.id}`;
    this.carouselItem = this.createCarouselItem();
    this.blobScene = new BlobScene(this.blobCanvasId);
    this.attachEventListeners();
    this.updateSensorData();
  }

  /**
   * Fetches and updates the device's sensor data.
   * Updates both the blob animation and UI elements if the device is active.
   *
   * @async
   * @returns {Promise<void>}
   */
  async updateSensorData() {
    this.sensorData = await ApiClient.fetchLatestDeviceReadings(this.id);
    if (!this.sensorData || !this.sensorData.length) return;

    const pressure =
      parseFloat(this.getSensorValue("BMP180 Pressure Sensor")) || null;
    const temperature =
      parseFloat(this.getSensorValue("DHT11 Temperature Sensor")) || null;
    const humidity =
      parseFloat(this.getSensorValue("DHT11 Humidity Sensor")) || null;
    const light =
      parseFloat(this.getSensorValue("BH1750 Light Sensor")) || null;

    this.blobScene.updateAppearance(pressure, temperature, humidity);

    if (this.isActive) {
      this.refreshUI();
      this.backgroundScene.updateAppearance(
        pressure,
        temperature,
        humidity,
        light,
      );
    }
  }

  /**
   * Extracts a specific sensor value from the sensor data array.
   *
   * @param {string} sensorType - The sensor type to retrieve.
   * @returns {number|undefined} The sensor value or undefined if not found.
   */
  getSensorValue(sensorType) {
    return this.sensorData.find((sensor) =>
      sensor.sensor_name.includes(sensorType),
    )?.value;
  }

  /**
   * Creates a carousel item representing the device.
   *
   * @returns {HTMLElement|null} The created carousel item or null if the container is not found.
   */
  createCarouselItem() {
    const carousel = ElementRegistry.get("carousel");
    if (!carousel) return null;

    const item = document.createElement("div");
    item.classList.add("carousel-item");
    item.dataset.deviceId = this.id;
    item.innerHTML = `
      <div class="title">
        <h2>${this.name}</h2>
        <code>${this.key}</code>
      </div>
      <div class="data"><ul></ul></div>
      <div class="blob-container" id="${this.blobCanvasId}"></div>
    `;
    carousel.appendChild(item);
    return item;
  }

  /**
   * Attaches event listeners to handle device activation within the carousel.
   */
  attachEventListeners() {
    document.addEventListener("carouselChange", (event) => {
      this.isActive = event.detail.activeDeviceId === this.id;
      if (this.isActive) {
        this.startUpdating();
      } else {
        this.stopUpdating();
      }
    });
  }

  /**
   * Generates the sensor measurement list for UI updates.
   *
   * @returns {string} The generated HTML string representing sensor data.
   */
  generateMeasurementList() {
    return this.sensorData
      .map(
        ({ sensor_name: sensorName, sensor_unit: sensorUnit, value }) => `
      <li>
        <div class="data-container">
          <p class="parameter">${sensorName}</p>
          <p class="value">${value} ${sensorUnit || ""}</p>
        </div>
      </li>
    `,
      )
      .join("");
  }

  /**
   * Updates the UI with the latest sensor data.
   */
  refreshUI() {
    if (this.carouselItem) {
      const dataList = this.carouselItem.querySelector(".data ul");
      if (dataList) {
        dataList.innerHTML = this.generateMeasurementList();
      }
    }
  }

  /**
   * Starts fetching sensor data at regular intervals.
   *
   * @async
   * @returns {Promise<void>}
   */
  async startUpdating() {
    if (this.updateInterval) return;
    await this.updateSensorData();
    this.updateInterval = setInterval(() => this.updateSensorData(), 2500);
  }

  /**
   * Stops the periodic fetching of sensor data.
   */
  stopUpdating() {
    if (this.updateInterval) {
      clearInterval(this.updateInterval);
      this.updateInterval = null;
    }
  }
}
