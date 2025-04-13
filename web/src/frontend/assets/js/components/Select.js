import ElementRegistry from "../helpers/ElementRegistry";
import Utils from "../helpers/Utils";
import WebSocketClient from "../helpers/WebSocketClient";

/**
 * Manages the selection menu, allowing users to switch between
 * different views of the application. Uses ElementRegistry to
 * efficiently manage UI elements and their visibility.
 *
 * @class Select
 * @author Yanis Deplazes
 * date 10.03.2025
 */
export default class Select {
  /**
   * The selection menu element.
   * @type {HTMLElement|null}
   * @private
   */
  select;

  /**
   * Initializes the selection menu and retrieves necessary UI elements.
   * If the select element is not found, logs an error.
   */
  constructor() {
    this.select = ElementRegistry.get("select");

    if (!this.select) {
      console.error("Select element not found.");
      return;
    }
    this.init();
  }

  /**
   * Renders the selection menu and makes it visible.
   * Calls `renderSelectionMenu()` to generate menu options.
   */
  init() {
    this.renderSelectionMenu();
    Utils.show(this.select);
  }

  /**
   * Dynamically creates the selection menu with available options.
   * Clears existing elements and appends new menu items.
   */
  renderSelectionMenu() {
    const menuItems = [
      {
        text: "Installation",
        action: () => this.handleSelection("installation"),
      },
      { text: "Overview", action: () => this.handleSelection("overview") },
    ];

    this.select.innerHTML = ""; // Clear existing elements
    menuItems.forEach(({ text, action }) => {
      const div = document.createElement("div");
      div.textContent = text;
      div.addEventListener("click", action);
      this.select.appendChild(div);
    });
  }

  /**
   * Handles the user selection and updates the UI accordingly.
   *
   * @param {string} option - The selected menu option.
   */
  handleSelection(option) {
    Utils.hide(this.select);

    switch (option) {
      case "installation":
        Select.showInstallation();
        break;
      case "overview":
        Select.showOverview();
        break;
      default:
        console.error("Unknown selection:", option);
    }
  }

  /**
   * Displays the installation view, updates UI elements, and triggers device updates.
   * Hides the background and applies the "installation" class to the carousel.
   */
  static async showInstallation() {
    Utils.show(ElementRegistry.get("carousel"));
    Utils.hide(ElementRegistry.get("background"));

    const carousel = ElementRegistry.get("carousel");
    if (carousel) {
      carousel.classList.add("installation");
    }

    WebSocketClient.connect();
  }

  /**
   * Displays the overview view and ensures all relevant UI elements are visible.
   */
  static showOverview() {
    Utils.show(ElementRegistry.get("header"));
    Utils.show(ElementRegistry.get("footer"));
    Utils.show(ElementRegistry.get("carousel"));

    WebSocketClient.connect();
  }
}
