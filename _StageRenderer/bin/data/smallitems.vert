uniform float white;

void main() {
  if (gl_Color.y > 0.0) {
    gl_Position = ftransform();
    gl_FrontColor = gl_Color;
    gl_FrontColor.a = white;
    gl_PointSize = 3.0;
  } else {
    gl_Position = vec4(0.0);
    gl_FrontColor = vec4(0.0);
    gl_PointSize = 0.0;
  }
}
