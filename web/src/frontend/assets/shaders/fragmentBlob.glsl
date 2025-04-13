uniform float u_intensity;
uniform float u_time;
uniform vec3 u_color1;
uniform vec3 u_color2;

varying vec2 vUv;
varying float vDisplacement;

void main() {
    float distort = 2.0 * vDisplacement * u_intensity * sin(vUv.y * 10.0 + u_time);
    vec3 color = mix(u_color1, u_color2, (1.0 - distort));
    gl_FragColor = vec4(color, 1.0);
}