import * as THREE from "three";
import { OrbitControls } from "three/examples/jsm/controls/OrbitControls";
import vertexShader from "../../shaders/vertexBlob.glsl";
import fragmentShader from "../../shaders/fragmentBlob.glsl";
import Utils from "../helpers/Utils";
import Settings from "../helpers/Settings";

/**
 * Represents a **3D animated Blob** using Three.js.
 * The blob dynamically changes **intensity, speed, and colors**
 * based on real-time **sensor data**.
 *
 * @class BlobScene
 * @author Yanis Deplazes
 * date 10.03.2025
 */
export default class BlobScene {
  /**
   * The container element where the Three.js scene is rendered.
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
   * The orbit controls for camera interaction.
   * @type {OrbitControls}
   * @private
   */
  controls;

  /**
   * The animated blob mesh.
   * @type {THREE.Mesh}
   * @private
   */
  blob;

  /**
   * Shader uniforms controlling animation behavior.
   * @type {object}
   * @private
   */
  uniforms;

  /**
   * Target values for smooth animations.
   * @type {number}
   * @private
   */
  targetIntensity;

  targetSpeed;

  targetColor1;

  targetColor2;

  /**
   * Controls the smoothness of transitions.
   * @type {number}
   * @private
   * @default 0.01
   */
  lerpSpeed = 0.01;

  /**
   * Creates a new **BlobScene** instance.
   *
   * @param {string} containerId - The **ID** of the HTML container where the scene will be rendered.
   */
  constructor(containerId) {
    this.container = document.getElementById(containerId);

    if (!this.container) {
      console.error(`Container with ID '${containerId}' not found`);
      return;
    }

    this.containerId = containerId;

    // Initialize Scene Components
    this.initScene();
    this.initCamera();
    this.initRenderer();
    this.initControls();
    this.initBlob();
    this.resize();
    this.animate();

    // Handle Window Resizing
    window.addEventListener("resize", () => this.resize());
  }

  /**
   * Initializes the **Three.js Scene**.
   */
  initScene() {
    this.scene = new THREE.Scene();
  }

  /**
   * Initializes the **Camera** with a fixed **1:1 aspect ratio**.
   */
  initCamera() {
    this.camera = new THREE.PerspectiveCamera(75, 1, 0.1, 1000);
    this.camera.position.set(0, 0, 5);
  }

  /**
   * Initializes the **WebGL Renderer**.
   */
  initRenderer() {
    this.renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });
    this.container.appendChild(this.renderer.domElement);
  }

  /**
   * Initializes **Orbit Controls** for user interaction.
   */
  initControls() {
    this.controls = new OrbitControls(this.camera, this.renderer.domElement);
    this.controls.enableZoom = false;
    this.controls.enablePan = false;
    this.controls.enableDamping = true;
  }

  /**
   * Creates and adds the **animated blob mesh** to the scene.
   */
  initBlob() {
    const geometry = new THREE.IcosahedronGeometry(2, 20); // Spherical shape

    this.uniforms = {
      u_time: { value: 0.0 },
      u_intensity: { value: 0.3 }, // Controls blob deformation
      u_speed: { value: 0.1 }, // Controls animation speed
      u_color1: { value: new THREE.Color(0xff5f6d) }, // Base color
      u_color2: { value: new THREE.Color(0x00f7ff) }, // Secondary color
    };

    const material = new THREE.ShaderMaterial({
      uniforms: this.uniforms,
      vertexShader,
      fragmentShader,
      side: THREE.DoubleSide,
    });

    this.blob = new THREE.Mesh(geometry, material);
    this.scene.add(this.blob);

    // **Smoothing Targets** (For **lerp** interpolation)
    this.targetIntensity = this.uniforms.u_intensity.value;
    this.targetSpeed = this.uniforms.u_speed.value;
    this.targetColor1 = this.uniforms.u_color1.value.clone();
    this.targetColor2 = this.uniforms.u_color2.value.clone();
  }

  /**
   * Handles **resizing** to maintain a **1:1 aspect ratio**.
   */
  resize() {
    const size = Math.min(
      this.container.clientWidth,
      this.container.clientHeight,
    );
    this.camera.aspect = 1;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(size, size);
  }

  /**
   * **Animation Loop**
   * - Updates **shader time**
   * - Smoothly **transitions between target values**
   * - Renders the scene on each frame
   */
  animate() {
    // Detect if container width has changed
    if (this.container.clientWidth !== this.previousContainerWidth) {
      this.resize();
      this.previousContainerWidth = this.container.clientWidth;
    }

    this.uniforms.u_time.value = performance.now() * 0.001;

    // **Smooth Transitions** using `lerp`
    this.uniforms.u_intensity.value +=
      (this.targetIntensity - this.uniforms.u_intensity.value) * this.lerpSpeed;
    this.uniforms.u_speed.value +=
      (this.targetSpeed - this.uniforms.u_speed.value) * this.lerpSpeed;
    this.uniforms.u_color1.value.lerp(this.targetColor1, this.lerpSpeed);
    this.uniforms.u_color2.value.lerp(this.targetColor2, this.lerpSpeed);

    this.controls.update();
    this.renderer.render(this.scene, this.camera);
    requestAnimationFrame(() => this.animate());
  }

  /**
   * Updates the Blob Appearance based on real-time sensor data.
   * @param {number} pressure - Current atmospheric pressure (hPa).
   * @param {number} temperature - Current temperature (°C).
   * @param {number} humidity - Current relative humidity (%).
   */
  updateAppearance(pressure, temperature, humidity) {
    if (typeof pressure === "number") {
      this.setIntensity(pressure);
      this.setSpeed(pressure);
    }
    if (typeof temperature === "number") {
      this.setColor1(temperature);
      if (typeof humidity === "number") {
        this.setColor2(temperature, humidity);
      }
    }
  }

  /**
   * Adjusts blob intensity based on atmospheric pressure.
   * Lower pressure → higher intensity (chaotic visuals),
   * Higher pressure → lower intensity (calm visuals).
   * @param {number} pressure - Atmospheric pressure in hPa.
   */
  setIntensity(pressure) {
    const pressureMin = Settings.get("pressureMin");
    const pressureMax = Settings.get("pressureMax");

    const intensityMin = Settings.get("blobIntensityMin");
    const intensityMax = Settings.get("blobIntensityMax");

    const norm = Utils.clamp(
      (pressureMax - pressure) / (pressureMax - pressureMin),
      0,
      1,
    );
    this.targetIntensity = intensityMin + (intensityMax - intensityMin) * norm;
  }

  /**
   * Adjusts blob animation speed based on atmospheric pressure.
   * Lower pressure → faster motion (energetic),
   * Higher pressure → slower motion (calmer).
   * @param {number} pressure - Atmospheric pressure in hPa.
   */
  setSpeed(pressure) {
    const pressureMin = Settings.get("pressureMin");
    const pressureMax = Settings.get("pressureMax");
    const speedMin = Settings.get("blobSpeedMin");
    const speedMax = Settings.get("blobSpeedMax");

    const norm = Utils.clamp(
      (pressureMax - pressure) / (pressureMax - pressureMin),
      0,
      1,
    );
    this.targetSpeed = speedMin + (speedMax - speedMin) * norm;
  }

  /**
   * Updates the primary blob color based on temperature.
   * Warmer or cooler tones reflect the temperature visually.
   * @param {number} temperature - Temperature in °C.
   */
  setColor1(temperature) {
    this.targetColor1.setStyle(Utils.getPrimaryColor(temperature));
  }

  /**
   * Updates the secondary blob color based on both temperature and humidity.
   * Adds complexity to the color blending based on conditions.
   * @param {number} temperature - Temperature in °C.
   * @param {number} humidity - Relative humidity in %.
   */
  setColor2(temperature, humidity) {
    this.targetColor2.setStyle(Utils.getSecondaryColor(temperature, humidity));
  }
}
