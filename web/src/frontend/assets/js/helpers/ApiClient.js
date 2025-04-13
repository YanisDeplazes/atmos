/**
 * Handles communication with the backend API to fetch and update data dynamically.
 * Provides methods for retrieving sensor readings, device lists, and application settings.
 *
 * @class ApiClient
 * @author Yanis Deplazes
 * date 10.03.2025
 */
export default class ApiClient {
  /**
   * Base URL for API requests, dynamically set based on the current hostname.
   * @type {string}
   * @constant
   */
  static API_BASE_URL = `${window.location.protocol}//${window.location.hostname}/api`;

  /**
   * Fetches the latest sensor data for a specific device.
   *
   * @async
   * @param {string} deviceId - The unique identifier of the device.
   * @returns {Promise<object[] | null>} Resolves to an array of sensor readings, or `null` if an error occurs.
   */
  static async fetchLatestDeviceReadings(deviceId) {
    try {
      const response = await fetch(
        `${this.API_BASE_URL}/views/LatestDeviceReadings?device_id=${deviceId}`,
      );

      if (!response.ok) {
        throw new Error(
          `Failed to fetch sensor data (Status: ${response.status})`,
        );
      }

      const data = await response.json();
      return Array.isArray(data) ? data : null;
    } catch (error) {
      console.error("Error fetching latest device readings:", error);
      return null;
    }
  }

  /**
   * Retrieves the current index from the installation endpoint.
   *
   * @async
   * @returns {Promise<number|null>} Resolves to the current index, or `null` if an error occurs.
   */
  static async fetchCurrentIndex() {
    try {
      const response = await fetch(`${this.API_BASE_URL}/installation/index`);

      if (!response.ok) {
        throw new Error(
          `Failed to fetch current index (Status: ${response.status})`,
        );
      }

      const data = await response.json();
      return typeof data.index === "number" ? data.index : null;
    } catch (error) {
      console.error("Error fetching installation index:", error);
      return null;
    }
  }

  /**
   * Fetches the list of available devices.
   *
   * @async
   * @returns {Promise<object[]>} Resolves to an array of device objects. Returns an empty array if an error occurs.
   */
  static async fetchDevices() {
    try {
      const response = await fetch(`${this.API_BASE_URL}/device`);

      if (!response.ok) {
        throw new Error(`Failed to fetch devices (Status: ${response.status})`);
      }

      const data = await response.json();
      return Array.isArray(data) ? data : [];
    } catch (error) {
      console.error("Error fetching devices:", error);
      return [];
    }
  }

  /**
   * Updates a specific setting in the database.
   *
   * @async
   * @param {string} id - The unique identifier of the setting.
   * @param {number|string} value - The new value for the setting.
   * @returns {Promise<boolean>} Resolves to `true` if the update was successful, `false` otherwise.
   */
  static async updateSetting(id, value) {
    try {
      const response = await fetch(`${this.API_BASE_URL}/setting/${id}`, {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ value }),
      });

      if (!response.ok) {
        throw new Error(
          `Failed to update setting (Status: ${response.status})`,
        );
      }

      return true;
    } catch (error) {
      console.error("Error updating setting:", error);
      return false;
    }
  }

  /**
   * Retrieves application settings from the database.
   *
   * @async
   * @returns {Promise<object>} Resolves to an object containing key-value settings. Returns an empty object if an error occurs.
   */
  static async getSettings() {
    try {
      const response = await fetch(`${this.API_BASE_URL}/setting`);

      if (!response.ok) {
        throw new Error(
          `Failed to fetch settings (Status: ${response.status})`,
        );
      }

      const data = await response.json();
      return data && typeof data === "object" ? data : {};
    } catch (error) {
      console.error("Error fetching settings:", error);
      return {};
    }
  }
}
