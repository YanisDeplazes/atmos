varying vec2 vUv;
varying float vElevation;
uniform vec3 uColor1;
uniform vec3 uColor2;
uniform float uBrightness; 

void main() {
    // Ensure vElevation is between 0 and 1 for smooth color transitions
    float elevationFactor = clamp(vElevation, 0.0, 1.0);

    // Use smoothstep for smoother transition
    vec3 color = mix(uColor1, uColor2, smoothstep(0.0, 1.0, elevationFactor));

    // Apply brightness scaling
    color *= uBrightness;

    gl_FragColor = vec4(color, 1.0);
}
