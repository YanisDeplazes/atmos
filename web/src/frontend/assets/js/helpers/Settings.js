import ApiClient from "./ApiClient";
import ElementRegistry from "./ElementRegistry";

/**
 * Manages application settings by dynamically fetching and updating configuration values from the API.
 * Converts setting keys from snake_case to camelCase for easier access in JavaScript.
 * Also dynamically updates the settings UI and allows saving changes back to the database.
 *
 * @class Settings
 * @author Yanis Deplazes
 * date 10.03.2025
 */
export default class Settings {
  /**
   * Represents a single setting with an ID and value.
   *
   * @typedef {object} Setting
   * @property {number} id - The unique setting identifier.
   * @property {number} value - The numerical value of the setting.
   */

  /**
   * Stores dynamically loaded settings from the API.
   * Keys are formatted in camelCase for easier access.
   *
   * @type {Record<string, Setting>}
   */
  static settings = {};

  /**
   * Fetches settings from the API and dynamically stores them.
   * Updates the settings UI with input fields allowing user modifications.
   *
   * @returns {Promise<void>} Resolves when settings are loaded and displayed.
   */
  static async loadSettings() {
    ElementRegistry.register(
      "settings",
      document.querySelector("section.settings"),
    );

    const settingsContainer = ElementRegistry.get("settings");

    try {
      const data = await ApiClient.getSettings();

      // Validate that the response contains an array of settings
      if (Array.isArray(data)) {
        this.settings = {};
        settingsContainer.innerHTML = ""; // Clear previous settings UI

        data.forEach(({ id, key, value, default_value: defaultValue }) => {
          // Convert key from snake_case to camelCase
          const camelCaseKey = key.replace(/_([a-z])/g, (_, char) =>
            char.toUpperCase(),
          );

          // Use the stored value if available, otherwise fallback to the default value
          this.settings[camelCaseKey] = {
            id,
            value:
              value !== null ? parseFloat(value) : parseFloat(defaultValue),
          };

          // Create input fields dynamically in the UI
          const settingDiv = document.createElement("div");
          settingDiv.classList.add("setting-item");

          const label = document.createElement("label");
          label.textContent = camelCaseKey;
          label.setAttribute("for", camelCaseKey);

          const input = document.createElement("input");
          input.type = "number"; // Assuming all values are numeric
          input.id = camelCaseKey;
          input.name = camelCaseKey;
          input.value = this.settings[camelCaseKey].value;
          input.step = "0.1";

          // Update settings map on change
          input.addEventListener("input", (event) => {
            this.settings[camelCaseKey].value = parseFloat(event.target.value);
          });

          settingDiv.appendChild(label);
          settingDiv.appendChild(input);
          settingsContainer.appendChild(settingDiv);
        });

        // Add submit button
        const submitButton = document.createElement("button");
        submitButton.textContent = "Save Settings";
        submitButton.addEventListener("click", () => this.submitSettings());
        settingsContainer.appendChild(submitButton);
      }
    } catch (error) {
      console.error("Failed to fetch settings:", error);
    }
  }

  /**
   * Sends updated settings to the API to be stored in the database.
   *
   * @returns {Promise<void>} Resolves when all settings are successfully updated.
   */
  static async submitSettings() {
    try {
      const updatePromises = Object.values(this.settings).map(({ id, value }) =>
        ApiClient.updateSetting(id, value),
      );
      await Promise.all(updatePromises);
    } catch (error) {
      console.error("Error updating settings:", error);
    }
  }

  /**
   * Retrieves a setting value from the loaded settings.
   *
   * @param {string} key - The setting key in camelCase format.
   * @returns {number|null} The corresponding setting value or null if not found.
   */
  static get(key) {
    return this.settings[key]?.value ?? null;
  }
}

// Load settings asynchronously on startup
Settings.loadSettings();
