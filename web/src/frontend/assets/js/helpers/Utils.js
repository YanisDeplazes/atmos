import Settings from "./Settings";

/**
 * Utility class providing helper functions for value clamping,
 * color interpolation based on temperature, humidity-based adjustments,
 * and DOM element visibility manipulation.
 *
 * @class Utils
 * @author Yanis Deplazes
 * date 10.03.2025
 */
export default class Utils {
  /**
   * Clamps a value within a specified range.
   *
   * @param {number} value - The value to be clamped.
   * @param {number} min - The minimum allowed value.
   * @param {number} max - The maximum allowed value.
   * @returns {number} The clamped value.
   */
  static clamp(value, min, max) {
    return Math.min(max, Math.max(min, value));
  }

  /**
   * Determines the primary color based on temperature using smooth interpolation.
   *
   * @param {number} temperature - The temperature value.
   * @returns {string} RGB color string (e.g., `"rgb(255, 217, 0)"`).
   */
  static getPrimaryColor(temperature) {
    const temperatureMax = Settings.get("temperatureMax");
    const temperatureMin = Settings.get("temperatureMin");
    const threshold = Settings.get("colorTemperatureThreshold");

    if (temperature >= threshold) {
      // Warm range: Yellow to Magenta
      return this.interpolateColor(temperature, [
        { temp: threshold, color: [255, 217, 0] }, // Yellow (#FFD900)
        { temp: temperatureMax, color: [255, 0, 171] }, // Magenta (#FF00AB)
      ]);
    }
    // Cold range: Cyan to Deep Blue
    return this.interpolateColor(temperature, [
      { temp: temperatureMin, color: [7, 14, 136] }, // Deep Blue (#070E88)
      { temp: threshold, color: [0, 247, 255] }, // Cyan (#00F7FF)
    ]);
  }

  /**
   * Represents a color stop in a gradient.
   *
   * @typedef {object} ColorStop
   * @property {number} temp - The temperature value.
   * @property {number[]} color - RGB color array in the format [r, g, b].
   */

  /**
   * Interpolates between color stops based on a given value.
   *
   * @param {number} value - The input value (e.g., temperature).
   * @param {ColorStop[]} stops - Array of color stops with temperature and RGB values.
   * @returns {string} Interpolated RGB color string.
   */
  static interpolateColor(value, stops) {
    if (value <= stops[0].temp) return `rgb(${stops[0].color.join(",")})`;
    if (value >= stops[stops.length - 1].temp)
      return `rgb(${stops[stops.length - 1].color.join(",")})`;

    let lower = stops[0];
    let upper = stops[stops.length - 1];
    for (let i = 0; i < stops.length - 1; i + 1) {
      if (value >= stops[i].temp && value <= stops[i + 1].temp) {
        lower = stops[i];
        upper = stops[i + 1];
        break;
      }
    }

    const ratio = (value - lower.temp) / (upper.temp - lower.temp);
    const interpolatedColor = lower.color.map((start, i) =>
      Math.round(start + ratio * (upper.color[i] - start)),
    );
    return `rgb(${interpolatedColor.join(",")})`;
  }

  /**
   * Computes a secondary color based on temperature and humidity.
   * The brightness is adjusted by blending humidity influence into the primary color.
   *
   * @param {number} temperature - The temperature value.
   * @param {number} humidity - Humidity percentage (0-100).
   * @returns {string} Adjusted RGB color string.
   */
  static getSecondaryColor(temperature, humidity) {
    const baseColor = Utils.getPrimaryColor(temperature)
      .match(/\d+/g)
      .map(Number);
    const humidityFactor = Utils.clamp(humidity / 100, 0, 1);
    const colorHumidityBrightnessFactor = Settings.get(
      "colorHumidityBrightnessFactor",
    );

    return `rgb(${baseColor
      .map((c) =>
        Math.min(
          255,
          Math.round(
            c + (255 - c) * humidityFactor * colorHumidityBrightnessFactor,
          ),
        ),
      )
      .join(",")})`;
  }

  /**
   * Hides a given DOM element by adding a "hidden" CSS class.
   *
   * @param {HTMLElement} element - The DOM element to hide.
   */
  static hide(element) {
    if (!element) {
      return;
    }
    element.classList.add("hidden");
  }

  /**
   * Shows a given DOM element by removing the "hidden" CSS class.
   *
   * @param {HTMLElement} element - The DOM element to show.
   */
  static show(element) {
    if (!element) {
      console.error("Element not found.");
      return;
    }
    element.classList.remove("hidden");
  }

  /**
   * Toggles the visibility of a registered element.
   *
   * @param {HTMLElement} element - The DOM element to toggle.
   */
  static toggle(element) {
    if (!element) {
      console.error("Element not found.");
      return;
    }
    element.classList.toggle("hidden");
  }

  /**
   * Generates a random value within the given range.
   *
   * @param {number} min - The minimum value.
   * @param {number} max - The maximum value.
   * @returns {number} A random value between min and max.
   */
  static getRandomValue(min, max) {
    return Math.random() * (max - min) + min;
  }
}
