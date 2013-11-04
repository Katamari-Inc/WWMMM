uniform samplerCube envMap;

varying vec3 r;

void main(void) {
  // gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
  // gl_FragColor = vec4(normalize(normal) * 0.5 + 0.5, 1.0);
  vec3 envColor = vec3(textureCube(envMap, r));
  gl_FragColor = vec4(envColor, 1.0);
}
