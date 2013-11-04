uniform mat4 modelMatrix;

varying vec4 vPos;

void main() {
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
  vPos = modelMatrix * gl_Vertex;
}
