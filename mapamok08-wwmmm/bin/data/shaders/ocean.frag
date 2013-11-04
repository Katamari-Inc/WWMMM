uniform sampler2D tex;
uniform float white;
uniform float visibility;

void main() {
  // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  gl_FragColor = mix(texture2D(tex, gl_TexCoord[0].st), vec4(1.0), white);
}
