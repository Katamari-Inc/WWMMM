uniform sampler2D tex;

varying vec4 vPos;

const float w = 910.0 / 2.0;
const float h = 610.0 / 2.0;

void main() {
  // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  // gl_FragColor = texture2D(tex, gl_TexCoord[0].st);
  if (vPos.z < -h || h < vPos.z || vPos.x < -w || w < vPos.x) discard;
  gl_FragColor = gl_Color;
}
