import ApiClient from "./ApiClient";

/**
 * Handles **real-time communication** with the backend via **WebSocket**.
 * Automatically updates the active index of the carousel across all connected clients
 * based on push events from the NGINX Push Stream module.
 *
 * @class WebSocketClient
 * @author Yanis Deplazes
 * @date 15.03.2025
 */
export default class WebSocketClient {
  /**
   * WebSocket connection instance.
   * @type {WebSocket|null}
   * @static
   */
  static socket = null;

  /**
   * Reference to the Carousel instance (for updating active index).
   * @type {Carousel|null}
   * @static
   */
  static carousel = null;

  /**
   * WebSocket endpoint URL, dynamically constructed from the current location.
   * Uses `wss://` or `ws://` depending on the protocol.
   * @type {string}
   * @static
   * @readonly
   */
  static URL = `${window.location.protocol === "https:" ? "wss:" : "ws:"}//${window.location.hostname}/api/installation/ws`;

  /**
   * Configures the WebSocket client with the required Carousel instance.
   * Must be called **before** `connect()`.
   *
   * @param {Carousel} carousel - The instance of the Carousel to control.
   */
  static setup(carousel) {
    this.carousel = carousel;
  }

  /**
   * Establishes the WebSocket connection and subscribes to server-sent messages.
   * Automatically sets the initial active index from the backend API and listens
   * for incoming `index` updates.
   *
   * Handles:
   * - Receiving new index via JSON message: `{ "index": <number> }`
   * - Applying index to the carousel
   * - Handling connection errors gracefully
   *
   * @async
   * @returns {Promise<void>}
   */
  static async connect() {
    if (!this.carousel || !this.URL) {
      console.error("WebSocketClient not configured. Call setup() first.");
      return;
    }

    this.socket = new WebSocket(this.URL);

    // Handle incoming messages
    this.socket.addEventListener("message", (event) => {
      try {
        const data = JSON.parse(event.data);
        if (data?.index !== undefined && this.carousel) {
          this.carousel.setActiveIndex(data.index);
        }
      } catch (err) {
        console.warn("Invalid WebSocket payload", err);
      }
    });

    // Fetch current index from backend for initial sync
    const currentIndex = await ApiClient.fetchCurrentIndex();
    if (currentIndex !== null) {
      this.carousel.setActiveIndex(currentIndex);
    }

    // Catch connection errors
    this.socket.addEventListener("error", (err) => {
      console.error("WebSocket error:", err);
    });
  }

  /**
   * Terminates the active WebSocket connection and resets state.
   */
  static disconnect() {
    if (this.socket) {
      this.socket.close();
      this.socket = null;
    }
  }
}
