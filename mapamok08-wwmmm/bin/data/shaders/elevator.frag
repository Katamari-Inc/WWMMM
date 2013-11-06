uniform float tick;
uniform sampler2D random;
uniform float white;
uniform float visibility;

varying vec4 vPos;

const vec3 base = vec3(0.0, 0.64, 0.86);
const vec3 variation = vec3(0.0, 0.05, 0.08);

vec3 HSVtoRGB(float h, float s, float v) {
  if (s <= 0.) { return vec3 (v); }
  h = mod(h, 1.) * 6.;
  float c = v * s;
  float x = (1. - abs((mod(h, 2.) - 1.))) * c;
  float m = v-c;
  float r = 0.0;
  float g = 0.0;
  float b = 0.0;

  if (h < 1.) { r = c; g = x;b = 0.0;}
  else if (h < 2.) { r = x; g = c; b = 0.0; }
  else if (h < 3.) { r = 0.0; g = c; b = x; }
  else if (h < 4.) { r = 0.0; g = x; b = c; }
  else if (h < 5.) { r = x; g = 0.0; b = c; }
  else  { r = c; g = 0.0; b = x; }

  return vec3(r + m, g + m, b + m);
}

void main() {
  if (vPos.z < (clamp(visibility / 0.8, 0.0, 1.0) - 0.5) * 500.0) {
    vec3 hsv = base + sin(texture2D(random, vPos.xz / 100.0).rgb * 3.1415 + vec3(tick)) * variation;
    vec4 color = vec4(HSVtoRGB(hsv.x, hsv.y, hsv.z), 1.0);
    gl_FragColor = mix(color, vec4(1.0), white);
  } else {
    gl_FragColor = vec4(1.0);
  }
}
