varying vec3 r;
varying vec3 position;
varying vec3 normal;

void main(void) {
  vec4 p = gl_ModelViewMatrix * gl_Vertex;
  // position = p.xyz;
  vec3 v = p.xyz / p.w;
  vec3 n = normalize(gl_NormalMatrix * gl_Normal);
  r = (gl_ModelViewMatrixInverse * vec4(reflect(p.xyz, normalize(n)), 0.0)).xyz;
  // r = reflect(p.xyz, normalize(n));
  // normal = n;
  gl_Position = ftransform();
}
