import BlobScene from "./BlobScene";
import BackgroundScene from "./BackgroundScene";
import ElementRegistry from "../helpers/ElementRegistry";
import Utils from "../helpers/Utils";
import Settings from "../helpers/Settings";

/**
 * Manages the application's loading animation and prepares the environment.
 * Uses a dynamic blob animation during the loading phase and transitions
 * into the main UI after a specified duration.
 *
 * @class Loader
 * @author Yanis Deplazes
 * date 10.03.2025
 */
export default class Loader {
  /**
   * The loader element.
   * @type {HTMLElement|null}
   * @private
   */
  loader;

  /**
   * Instance of the BlobScene, responsible for animated visuals.
   * @type {BlobScene|null}
   * @private
   */
  blobScene;

  /**
   * Background scene instance.
   * @type {BackgroundScene}
   * @private
   */
  backgroundScene;

  /**
   * Interval ID for periodic updates to the blob animation.
   * @type {number|null}
   * @private
   */
  updateInterval;

  /**
   * Duration (in milliseconds) before transitioning to the main UI.
   * @type {number}
   * @constant
   * @default 5000
   * @private
   */
  displayDuration = 5000;

  /**
   * Frequency (in milliseconds) of blob updates.
   * @type {number}
   * @constant
   * @default 1000
   * @private
   */
  updateFrequency = 1000;

  /**
   * Callback function executed after loading is complete.
   * @type {Function}
   * @private
   */
  callback;

  /**
   * Creates a new Loader instance.
   *
   * @param {Function} callback - Function to execute after loading completes.
   */
  constructor(callback) {
    this.loader = ElementRegistry.get("loader");
    this.blobScene = null;
    this.updateInterval = null;
    this.callback = callback;
    this.init();
  }

  /**
   * Initializes the loader by creating the UI, starting the animation,
   * and scheduling the transition to the main UI.
   */
  init() {
    this.createHTML();
    Utils.show(ElementRegistry.get("loader"));
    this.blobScene = new BlobScene("intro-blob");
    this.backgroundScene = BackgroundScene.getInstance();
    this.startUpdatingBlob();

    setTimeout(() => {
      this.cleanup();
      if (this.callback) this.callback();
    }, this.displayDuration);
  }

  /**
   * Creates the loading screen UI elements.
   * Adds a blob animation and heading text.
   */
  createHTML() {
    const blobDiv = document.createElement("div");
    blobDiv.id = "intro-blob";
    const heading = document.createElement("h2");
    heading.style.textAlign = "center";
    heading.textContent = "Welcome to Atmos";
    this.loader.appendChild(blobDiv);
    this.loader.appendChild(heading);
  }

  /**
   * Starts periodically updating the blob's appearance based on simulated environment data.
   */
  startUpdatingBlob() {
    this.updateInterval = setInterval(() => {
      this.updateBlobAppearance();
    }, this.updateFrequency);
  }

  /**
   * Updates the blob and background animation using randomly generated values
   * that mimic environmental conditions.
   */
  updateBlobAppearance() {
    if (this.blobScene) {
      const pressure = Utils.getRandomValue(
        Settings.get("pressureMin"),
        Settings.get("pressureMax"),
      );
      const temperature = Utils.getRandomValue(
        Settings.get("temperatureMin"),
        Settings.get("temperatureMax"),
      );
      const humidity = Utils.getRandomValue(
        Settings.get("humidityMin"),
        Settings.get("humidityMax"),
      );
      const light = Utils.getRandomValue(
        Settings.get("luxMin"),
        Settings.get("luxMax"),
      );

      this.blobScene.updateAppearance(pressure, temperature, humidity);
      this.backgroundScene.updateAppearance(
        pressure,
        temperature,
        humidity,
        light,
      );
    }
  }

  /**
   * Cleans up the loading animation by stopping updates and hiding the loader UI.
   */
  cleanup() {
    this.stopUpdatingBlob();
    Utils.hide(ElementRegistry.get("loader"));
  }

  /**
   * Stops the periodic updates to the blob animation.
   */
  stopUpdatingBlob() {
    if (this.updateInterval) {
      clearInterval(this.updateInterval);
      this.updateInterval = null;
    }
  }
}
