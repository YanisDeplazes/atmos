/**
 * Manages global references to UI elements.
 * Provides an efficient way to register, retrieve, and manipulate DOM elements
 * without repeated queries.
 *
 * @class ElementRegistry
 * @author Yanis Deplazes
 * date 15.03.2025
 */
export default class ElementRegistry {
  /**
   * A Map storing registered elements by key.
   *
   * @type {Map<string, HTMLElement>}
   */
  static elements = new Map();

  /**
   * Registers an HTML element for easy access.
   *
   * @param {string} key - Unique identifier for the element.
   * @param {HTMLElement} element - The DOM element to register.
   * @throws {Error} If the provided element is not a valid `HTMLElement`.
   */
  static register(key, element) {
    if (!(element instanceof HTMLElement)) {
      console.error(
        `ElementRegistry: Attempted to register a non-HTMLElement for key '${key}'`,
      );
      return;
    }
    this.elements.set(key, element);
  }

  /**
   * Retrieves a previously registered element.
   *
   * @param {string} key - The identifier of the registered element.
   * @returns {HTMLElement|null} The element if found, otherwise `null`.
   */
  static get(key) {
    return this.elements.get(key) || null;
  }
}
