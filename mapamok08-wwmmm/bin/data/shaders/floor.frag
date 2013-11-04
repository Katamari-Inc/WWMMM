uniform sampler2D tex;
uniform float white;
uniform float visibility;

varying vec4 vPos;

void main() {
  if (vPos.z < (clamp(visibility / 0.8, 0.0, 1.0) - 0.5) * 500.0) {
    gl_FragColor = mix(texture2D(tex, gl_TexCoord[0].st), vec4(1.0), white);
  } else {
    gl_FragColor = vec4(1.0);
  }
}
