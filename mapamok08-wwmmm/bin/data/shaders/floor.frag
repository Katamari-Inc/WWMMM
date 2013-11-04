uniform sampler2D tex;
uniform float white;
uniform float visibility;

varying vec4 vPos;

void main() {
  if (vPos.z < (visibility - 0.5) * 500.0) {
    gl_FragColor = texture2D(tex, gl_TexCoord[0].st);
  } else {
    gl_FragColor = vec4(1.0);
  }
}
