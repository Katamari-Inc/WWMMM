//-----------------------------------------------------------------------------
// https://github.com/ashima/webgl-noise/blob/master/src/noise3D.glsl
vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 permute(vec4 x) { return mod289(((x*34.0)+1.0)*x); }
vec4 taylorInvSqrt(vec4 r) { return 1.79284291400159 - 0.85373472095314 * r; }

float snoise(vec3 v) {
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy;
  vec3 x3 = x0 - D.yyy;

  i = mod289(i);
  vec4 p = permute( permute( permute(
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

  float n_ = 0.142857142857;
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3) ) );
}


//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
vec3 RGBtoHSV(float r, float g, float b) {
  float max = max( max(r,g),b);
  float min = min(min(r,g),b);
  float hue;
  float saturation;
  float value = max;

  if (min == max) {
    hue = 0.;
    saturation = 0.;
  } else {
    float delta = (max - min);
    saturation = delta / max;

    if( r == max ) {
      hue = ( g - b ) / delta;
    } else if ( g == max ) {
      hue = 2. + ( ( b - r ) / delta );
    } else  {
      hue = 4. + ( ( r - g ) / delta );
    }

    hue /= 6.;

    if( hue < 0. ) {
      hue += 1.;
    }

    if( hue > 1. ) {
      hue -= 1.;
    }
  }
    return vec3(hue, saturation, value);
}


//-----------------------------------------------------------------------------

float rand(vec2 n) {
  return fract(sin(dot(n.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

uniform sampler2D tex;
uniform float tick;
uniform mat4 modelMatrix;
uniform float white;

varying vec4 vPos;

const vec2 size = vec2(910.0 + 800.0, 610.0);
const vec3 variation = vec3(0.0, 0.05, 0.08);

void main() {
  vec3 color = texture2D(tex, gl_Vertex.zx / size.yx + 0.5).rgb;
  float h = snoise(gl_Vertex.xyz * sin(tick) * .5);
  vec3 base = RGBtoHSV(color.r, color.g, color.b);
  float time = tick * 0.5;
  float n1 = snoise(gl_Color.rgb + vec3(0, 0, time));
  float n2 = snoise(gl_Color.gbr + vec3(0, time, 0.));
  vec3 hsv2 = base + variation * vec3(0., n1, n2);
  if (gl_Color.r < smoothstep(0.0, 1.0 + rand(gl_Color.rg + white) * 0.1 * max(1.0 - white, 0.0), white)) {
    gl_FrontColor = vec4(HSVtoRGB(hsv2.x, hsv2.y, hsv2.z), 1.0);
  } else {
    gl_FrontColor = vec4(1.0);
  }

  float n = snoise(gl_Vertex.xyz * 0.01 + tick * 0.1);
  vPos = gl_Vertex + n * vec4(10.0, 0.0, 10.0, 0.0);
  gl_Position = gl_ModelViewProjectionMatrix * vPos;
  // gl_Position = ftransform();
  // gl_TexCoord[0] = gl_MultiTexCoord0;
}
