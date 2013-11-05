uniform float radius;
uniform sampler2D palette;

// attribute vec3 center;

void main() {
  vec3 center = gl_Normal;
  float d = length(center);
  float s = d > 400.0 ? 0.0 : 1. - smoothstep(0., 100., abs(d - radius));
  vec3 p = (gl_Vertex.xyz - center) * s + center;
  gl_Position = gl_ModelViewProjectionMatrix * vec4(p, 1.);
  gl_FrontColor = texture2D(palette, vec2(mod(gl_Color.r + (d - radius) / 400., 1.), .5));
  // gl_FrontColor = vec4(d / 300.0, 0.0, 0.0, 1.0);
}
