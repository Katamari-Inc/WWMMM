uniform float white;
uniform float visibility;

varying vec4 vPos;

void main() {
  // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  // gl_FragColor = vec4(vPos.xyz, 1.0);
  // gl_FragColor = gl_Color;
  // gl_FragColor = vec4(vec3(vPos.y) / 65.0, 1.0);
  if (vPos.y < clamp(visibility - 0.8, 0.0, 1.0) * 5.0 * 65.0) {
    gl_FragColor = gl_Color;
  } else {
    gl_FragColor = vec4(1.0);
  }
}
