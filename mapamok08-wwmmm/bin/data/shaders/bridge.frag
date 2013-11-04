uniform float white;
uniform float visibility;

varying vec4 vPos;

const vec4 color = vec4(vec3(64, 148, 65) / 255.0, 1.0);

void main() {
  // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  if (vPos.z < (clamp(visibility / 0.8, 0.0, 1.0) - 0.5) * 500.0) {
    gl_FragColor = color;
  } else {
    gl_FragColor = vec4(1.0);
  }
}
