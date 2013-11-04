uniform mat4 modelMatrix;

varying vec4 vPos;

void main() {
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
  gl_FrontColor = gl_Color;
  vPos = modelMatrix * gl_Vertex;
}
