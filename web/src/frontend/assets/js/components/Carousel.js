/**
 * Manages a dynamically interactive image carousel with navigation dots.
 * Enables users to cycle through carousel items and updates the active device
 * for sensor data visualization.
 *
 * @class Carousel
 * @author Yanis Deplazes
 * date 10.03.2025
 */
export default class Carousel {
  /**
   * The carousel container element.
   * @type {HTMLElement|null}
   * @private
   */
  carousel;

  /**
   * The container holding navigation dots.
   * @type {HTMLElement|null}
   * @private
   */
  dotsContainer;

  /**
   * An array of all carousel items.
   * @type {HTMLElement[]}
   * @private
   */
  carouselItems = [];

  /**
   * An array of all navigation dots.
   * @type {HTMLElement[]}
   * @private
   */
  dots = [];

  /**
   * The index of the currently active carousel item.
   * @type {number}
   * @private
   */
  activeIndex = 0;

  /**
   * Creates a new Carousel instance.
   *
   * @param {string} carouselSelector - CSS selector for the carousel container.
   * @param {string} dotsContainerSelector - CSS selector for the navigation dots container.
   */
  constructor(carouselSelector, dotsContainerSelector) {
    this.carousel = document.querySelector(carouselSelector);
    this.dotsContainer = document.querySelector(dotsContainerSelector);

    if (!this.carousel || !this.dotsContainer) {
      console.error("Carousel elements not found.");
      return;
    }

    this.carouselItems = Array.from(
      this.carousel.querySelectorAll(".carousel-item"),
    );
    if (!this.carouselItems.length) {
      console.error("No carousel items found!");
      return;
    }

    this.createDotsList();
    this.dots = Array.from(this.dotsContainer.querySelectorAll("li"));
    this.init();
  }

  /**
   * Initializes the carousel by setting up event listeners and updating UI.
   */
  init() {
    this.updateCarousel();
    this.addEventListeners();
  }

  /**
   * Generates the navigation dots dynamically based on the number of carousel items.
   */
  createDotsList() {
    this.dotsContainer.innerHTML = "<ul></ul>";
    const dotsList = this.dotsContainer.querySelector("ul");
    this.carouselItems.forEach((_, index) => {
      const dot = document.createElement("li");
      dot.setAttribute("data-index", index);
      dotsList.appendChild(dot);
    });
  }

  /**
   * Updates the carousel to reflect the currently active item.
   * Adjusts positioning and triggers UI transitions.
   */
  updateCarousel() {
    const totalItems = this.carouselItems.length;
    this.carouselItems.forEach((item, index) => {
      item.classList.remove(
        "active",
        "prev",
        "next",
        "prev-hidden",
        "next-hidden",
      );

      if (index === this.activeIndex) {
        item.classList.add("active");
        const deviceId = item.getAttribute("data-device-id");
        if (deviceId) {
          document.dispatchEvent(
            new CustomEvent("carouselChange", {
              detail: { activeDeviceId: deviceId },
            }),
          );
        }
      } else if (index === (this.activeIndex - 1 + totalItems) % totalItems) {
        item.classList.add("prev");
      } else if (index === (this.activeIndex + 1) % totalItems) {
        item.classList.add("next");
      } else if (index === (this.activeIndex - 2 + totalItems) % totalItems) {
        item.classList.add("prev-hidden");
      } else if (index === (this.activeIndex + 2) % totalItems) {
        item.classList.add("next-hidden");
      }
    });
    this.updateDots();
  }

  /**
   * Updates the navigation dots to indicate the currently active carousel item.
   */
  updateDots() {
    this.dots.forEach((dot, index) => {
      dot.classList.toggle("active", index === this.activeIndex);
    });
  }

  /**
   * Moves the carousel to the previous item.
   */
  shiftLeft() {
    const newIndex =
      (this.activeIndex - 1 + this.carouselItems.length) %
      this.carouselItems.length;
    this.setActiveIndex(newIndex);
  }

  /**
   * Moves the carousel to the next item.
   */
  shiftRight() {
    const newIndex = (this.activeIndex + 1) % this.carouselItems.length;
    this.setActiveIndex(newIndex);
  }

  /**
   * Sets the active carousel index from external sources (e.g., WebSocket).
   * @param {number} index - The new active index.
   */
  setActiveIndex(index) {
    if (
      typeof index === "number" &&
      index >= 0 &&
      index < this.carouselItems.length
    ) {
      this.activeIndex = index;
      this.updateCarousel();
    }
  }

  /**
   * Adds event listeners for user interactions with the carousel and navigation dots.
   */
  addEventListeners() {
    this.carousel.addEventListener("click", (event) => {
      if (event.target.closest(".prev")) this.shiftLeft();
      else if (event.target.closest(".next")) this.shiftRight();
    });

    this.dots.forEach((dot) => {
      dot.addEventListener("click", () => {
        this.activeIndex = parseInt(dot.getAttribute("data-index"), 10);
        this.updateCarousel();
      });
    });
  }
}
