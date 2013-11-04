uniform float white;
uniform float visibility;

varying vec4 vPos;

void main() {
  // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  if (vPos.z < (visibility - 0.5) * 500.0) {
    gl_FragColor = vec4(vec3(64, 148, 65) / 255.0, 1.0);
  } else {
    gl_FragColor = vec4(1.0);
  }
}
