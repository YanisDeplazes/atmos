varying vec2 vUv;
varying float vElevation;
uniform float uTime;
uniform float uNoiseScale;
uniform float uHeight;
uniform float uSpeed; // Speed control

// Simplex-style random function
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Basic noise function
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

// Fractal Brownian Motion (FBM) for smooth, organic shapes
float fbm(vec2 st) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;

    for (int i = 0; i < 5; i++) { 
        value += amplitude * noise(st * frequency);
        frequency *= 1.8;
        amplitude *= 0.1;  // Lower amplitude to smooth out peaks
    }
    return value;
}

// Warping function for rounded shapes
float warp(vec2 st) {
    float base = fbm(st);
    float warpFactor = sin(base * 3.14) * 0.5 + 0.5;
    return mix(base, warpFactor, 0.8);
}

void main() {
    vUv = uv;

    // Create a more natural displacement over time
    float timeOffset = uTime * uSpeed * 0.05;
    float elevation = warp(uv * uNoiseScale + vec2(timeOffset, timeOffset)) * uHeight;

    vElevation = elevation; // Pass this to the fragment shader

    // Normalize vElevation between 0 and 1 for more dramatic height effects
    vElevation = (vElevation + 1.0) / (uHeight * 2.5); // Increased height range for more pronounced peaks

    vec3 newPosition = position + vec3(0.0, elevation, 0.0);
    gl_Position = projectionMatrix * modelViewMatrix * vec4(newPosition, 1.0);
}

  