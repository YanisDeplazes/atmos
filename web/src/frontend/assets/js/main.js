import "../styles/style.scss";
import BackgroundScene from "./components/BackgroundScene";
import Carousel from "./components/Carousel";
import Device from "./components/Device";
import ApiClient from "./helpers/ApiClient";
import Loader from "./components/Loader";
import Select from "./components/Select";
import ElementRegistry from "./helpers/ElementRegistry";
import Utils from "./helpers/Utils";
import WebSocketClient from "./helpers/WebSocketClient";

/**
 * **Main entry point** for the application.
 * Initializes the **background animation, IoT devices, and UI components**.
 * Uses the **ElementRegistry** for efficient UI management.
 *
 * @class App
 * @author Yanis Deplazes
 * date 15.03.2025
 */
export default class App {
  /**
   * The background scene instance.
   * @type {BackgroundScene}
   * @private
   */
  backgroundScene;

  /**
   * An array storing all IoT devices.
   * @type {Device[]}
   * @private
   */
  devices = [];

  /**
   * Initializes the application by **registering UI elements**
   * and setting up core components.
   */
  constructor() {
    App.registerUIElements();
  }

  /**
   * Main initialization method.
   * - **Initializes the Three.js background**
   * - **Loads IoT devices asynchronously**
   * - **Initializes UI components**
   *
   * @async
   * @returns {Promise<void>}
   */
  async init() {
    this.initBackground();
    await this.initDevices();
    App.initUI();
  }

  /**
   * Initializes the **Three.js animated background**.
   */
  initBackground() {
    this.backgroundScene = BackgroundScene.getInstance();
  }

  /**
   * Fetches and initializes all **available IoT devices**.
   *
   * @async
   * @returns {Promise<void>}
   */
  async initDevices() {
    const devicesData = await ApiClient.fetchDevices();
    if (!devicesData.length) {
      console.warn("No devices found.");
      return;
    }
    this.devices = devicesData.map((device) => new Device(device));
  }

  /**
   * **Registers key UI elements** into the `ElementRegistry`
   * for efficient **DOM access and management**.
   */
  static registerUIElements() {
    ElementRegistry.register(
      "background",
      document.querySelector("#background-container"),
    );
    ElementRegistry.register("header", document.querySelector("header"));
    ElementRegistry.register("footer", document.querySelector("footer"));
    ElementRegistry.register(
      "select",
      document.querySelector("section.select"),
    );
    ElementRegistry.register(
      "loader",
      document.querySelector("section.loader"),
    );
    ElementRegistry.register(
      "carousel",
      document.querySelector("section.carousel"),
    );
  }

  /**
   * **Initializes UI components** such as:
   * - `Loader` → Handles app startup animation
   * - `Select` → Manages the selection menu
   * - `Carousel` → Enables navigation through devices
   */
  static initUI() {
    this.loader = new Loader(() => new Select());
    this.carousel = new Carousel(".carousel", ".carousel-dots");

    WebSocketClient.setup(this.carousel);
    WebSocketClient.connect();

    // Toggle settings section visibility on button click
    document.querySelector(".settings-button").addEventListener("click", () => {
      Utils.toggle(ElementRegistry.get("settings"));
    });
  }
}

const appInstance = new App();
appInstance.init();
