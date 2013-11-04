uniform mat4 cameraMatrix;

varying vec3 r;

void main(void) {
  vec4 p = gl_ModelViewMatrix * gl_Vertex;  // 頂点位置
  vec3 v = p.xyz / p.w;                     // 視線ベクトル
  vec3 n = normalize(gl_NormalMatrix * gl_Normal);     // 法線ベクトル
  r = vec3(cameraMatrix * vec4(reflect(v, n), 0));
  gl_Position = ftransform();
}
