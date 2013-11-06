varying vec4 vPos;

void main() {
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
  vPos = gl_Vertex;
}
