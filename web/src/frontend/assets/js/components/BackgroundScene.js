import * as THREE from "three";
import vertexShader from "../../shaders/vertexBackground.glsl";
import fragmentShader from "../../shaders/fragmentBackground.glsl";
import Utils from "../helpers/Utils";
import Settings from "../helpers/Settings";

/**
 * Singleton class representing an animated **Three.js background**.
 * Dynamically adjusts based on environmental sensor data such as
 * **wind speed, pressure, temperature, humidity, and brightness**.
 *
 * @class BackgroundScene
 * @author Yanis Deplazes
 * date 10.03.2025
 */
export default class BackgroundScene {
  /**
   * Singleton instance of BackgroundScene.
   * @type {BackgroundScene|null}
   * @private
   */
  static instance = null;

  /**
   * The HTML container element where the scene is rendered.
   * @type {HTMLElement|null}
   * @private
   */
  container;

  /**
   * The Three.js scene.
   * @type {THREE.Scene}
   * @private
   */
  scene;

  /**
   * The Three.js camera.
   * @type {THREE.PerspectiveCamera}
   * @private
   */
  camera;

  /**
   * The WebGL renderer for the scene.
   * @type {THREE.WebGLRenderer}
   * @private
   */
  renderer;

  /**
   * The animated background mesh.
   * @type {THREE.Mesh}
   * @private
   */
  plane;

  /**
   * Shader uniforms controlling animation behavior.
   * @type {object}
   * @private
   */
  uniforms;

  /**
   * Targets for smooth transitions.
   * @type {number}
   * @private
   */
  targetHeight;

  targetNoiseScale;

  targetSpeed;

  targetColor1;

  targetColor2;

  targetBrightness;

  /**
   * Controls the smoothness of transitions.
   * @type {number}
   * @default 0.01
   * @private
   */
  lerpSpeed = 0.01;

  /**
   * Creates or returns the existing BackgroundScene instance.
   *
   * @param {string} containerId - The ID of the HTML container where the scene will be rendered.
   */
  constructor(containerId) {
    this.container = document.getElementById(containerId);
    this.initScene();
    this.initCamera();
    this.initRenderer();
    this.initBackground();
    this.resize();
    this.animate();

    window.addEventListener("resize", () => this.resize());

    BackgroundScene.instance = this;
  }

  /**
   * Initializes the Three.js scene.
   */
  initScene() {
    this.scene = new THREE.Scene();
  }

  /**
   * Initializes the camera with a fixed **perspective view**.
   */
  initCamera() {
    this.camera = new THREE.PerspectiveCamera(
      75,
      window.innerWidth / window.innerHeight,
      0.1,
      1000,
    );
    this.camera.position.set(0, 3, 0);
    this.camera.lookAt(0, 0, 0);
  }

  /**
   * Initializes the WebGL renderer.
   */
  initRenderer() {
    this.renderer = new THREE.WebGLRenderer({ antialias: true });
    this.container.appendChild(this.renderer.domElement);
  }

  /**
   * Creates and adds the **animated background plane**.
   */
  initBackground() {
    const geometry = new THREE.PlaneGeometry(10, 10, 300, 300);
    geometry.rotateX(-Math.PI / 2);

    this.uniforms = {
      uTime: { value: 0 },
      uNoiseScale: { value: 10.0 },
      uHeight: { value: 1.2 },
      uSpeed: { value: 1.0 },
      uColor1: { value: new THREE.Color(0x030637) },
      uColor2: { value: new THREE.Color(0x00f7ff) },
      uBrightness: { value: 1.0 },
    };

    const material = new THREE.ShaderMaterial({
      vertexShader,
      fragmentShader,
      uniforms: this.uniforms,
      side: THREE.DoubleSide,
      wireframe: false,
    });

    this.plane = new THREE.Mesh(geometry, material);
    this.scene.add(this.plane);

    // Animation smoothing targets
    this.targetHeight = this.uniforms.uHeight.value;
    this.targetNoiseScale = this.uniforms.uNoiseScale.value;
    this.targetSpeed = this.uniforms.uSpeed.value;
    this.targetColor1 = this.uniforms.uColor1.value.clone();
    this.targetColor2 = this.uniforms.uColor2.value.clone();
    this.targetBrightness = this.uniforms.uBrightness.value;
  }

  /**
   * Handles resizing to maintain **correct aspect ratio**.
   */
  resize() {
    this.renderer.setSize(window.innerWidth, window.innerHeight);
    this.camera.aspect = window.innerWidth / window.innerHeight;
    this.camera.updateProjectionMatrix();
  }

  /**
   * **Animation Loop**
   * - Updates **shader time**
   * - Smoothly **transitions between target values**
   * - Renders the scene on each frame
   */
  animate() {
    this.uniforms.uTime.value = performance.now() * 0.001;

    // Smooth Transitions using `lerp`
    this.uniforms.uHeight.value +=
      (this.targetHeight - this.uniforms.uHeight.value) * this.lerpSpeed;
    this.uniforms.uNoiseScale.value +=
      (this.targetNoiseScale - this.uniforms.uNoiseScale.value) *
      this.lerpSpeed;
    this.uniforms.uSpeed.value +=
      (this.targetSpeed - this.uniforms.uSpeed.value) * this.lerpSpeed;
    this.uniforms.uColor1.value.lerp(this.targetColor1, this.lerpSpeed);
    this.uniforms.uColor2.value.lerp(this.targetColor2, this.lerpSpeed);
    this.uniforms.uBrightness.value +=
      (this.targetBrightness - this.uniforms.uBrightness.value) *
      this.lerpSpeed;

    this.renderer.render(this.scene, this.camera);
    requestAnimationFrame(() => this.animate());
  }

  /**
   * Updates the background appearance based on live environmental sensor data.
   * @param {number} pressure - Atmospheric pressure in hPa.
   * @param {number} temperature - Temperature in °C.
   * @param {number} humidity - Relative humidity in %.
   * @param {number} light - Ambient light in lux.
   */
  updateAppearance(pressure, temperature, humidity, light) {
    if (typeof pressure === "number") {
      this.setNoiseScale(pressure);
      this.setSpeed(pressure);
    }
    if (typeof pressure === "number") {
      this.setHeight(pressure);
    }
    if (typeof temperature === "number") {
      this.setColor1(temperature);
    }
    if (typeof temperature === "number" && typeof humidity === "number") {
      this.setColor2(temperature, humidity);
    }
    if (typeof light === "number") {
      this.setBrightness(light);
    }
  }

  /**
   * Updates the background noise scale based on atmospheric pressure.
   * Lower pressure results in more turbulent, higher noise scale.
   * @param {number} pressure - Atmospheric pressure in hPa.
   */

  setNoiseScale(pressure) {
    const pressureMin = Settings.get("pressureMin");
    const pressureMax = Settings.get("pressureMax");
    const noiseScaleMin = Settings.get("backgroundNoiseScaleMin");
    const noiseScaleMax = Settings.get("backgroundNoiseScaleMax");

    const norm = Utils.clamp(
      (pressureMax - pressure) / (pressureMax - pressureMin),
      0,
      1,
    );
    this.targetNoiseScale =
      noiseScaleMin + (noiseScaleMax - noiseScaleMin) * norm;
  }

  /**
   * Adjusts the background vertical height based on atmospheric pressure.
   * Lower pressure raises the background to simulate uplift.
   * @param {number} pressure - Atmospheric pressure in hPa.
   */
  setHeight(pressure) {
    const pressureMin = Settings.get("pressureMin");
    const pressureMax = Settings.get("pressureMax");
    const heightMin = Settings.get("backgroundHeightMin");
    const heightMax = Settings.get("backgroundHeightMax");

    const norm = Utils.clamp(
      (pressureMax - pressure) / (pressureMax - pressureMin),
      0,
      1,
    );
    this.targetHeight = heightMin + (heightMax - heightMin) * norm;
  }

  /**
   * Adjusts background animation speed based on atmospheric pressure.
   * Lower pressure increases animation speed to simulate stronger winds.
   * @param {number} pressure - Atmospheric pressure in hPa.
   */
  setSpeed(pressure) {
    const pressureMin = Settings.get("pressureMin");
    const pressureMax = Settings.get("pressureMax");
    const speedMin = Settings.get("backgroundSpeedMin");
    const speedMax = Settings.get("backgroundSpeedMax");

    const norm = Utils.clamp(
      (pressureMax - pressure) / (pressureMax - pressureMin),
      0,
      1,
    );
    this.targetSpeed = speedMin + (speedMax - speedMin) * norm;
  }

  /**
   * Updates the cooler base color of the background based on temperature.
   * @param {number} temperature - Temperature in °C.
   */
  setColor1(temperature) {
    const colorString = Utils.getPrimaryColor(temperature);
    this.targetColor1.setStyle(colorString);
  }

  /**
   * Updates the warmer highlight color of the background using temperature and humidity.
   * @param {number} temperature - Temperature in °C.
   * @param {number} humidity - Relative humidity in %.
   */
  setColor2(temperature, humidity) {
    const colorString = Utils.getSecondaryColor(temperature, humidity);
    this.targetColor2.setStyle(colorString);
  }

  /**
   * Adjusts scene brightness based on ambient light in lux.
   * Lux is mapped to a normalized brightness range.
   * @param {number} lux - Ambient light level in lux.
   */
  setBrightness(lux) {
    const luxMin = Settings.get("luxMin");
    const luxMax = Settings.get("luxMax");
    const brightnessMin = Settings.get("backgroundBrightnessMin");
    const brightnessMax = Settings.get("backgroundBrightnessMax");

    const norm = Utils.clamp((lux - luxMin) / (luxMax - luxMin), 0, 1);
    this.targetBrightness = Utils.clamp(
      brightnessMin + (brightnessMax - brightnessMin) * norm,
      brightnessMin,
      brightnessMax,
    );
  }

  /**
   * **Static method** to get the **singleton instance**.
   * @param {string} containerId - The container ID (only needed for first initialization).
   * @returns {BackgroundScene} The singleton instance.
   */
  static getInstance(containerId = "background-container") {
    if (!BackgroundScene.instance) {
      BackgroundScene.instance = new BackgroundScene(containerId);
    }
    return BackgroundScene.instance;
  }
}
