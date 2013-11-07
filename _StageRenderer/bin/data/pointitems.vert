uniform float white;

const vec4 color = vec4(0.067, 0.765, 0.824, 1.0);

void main() {
  if (gl_Color.r > 0.0) {
    gl_Position = ftransform();
    gl_FrontColor = color;
    gl_FrontColor.a = white;
    gl_PointSize = gl_Color.g > 0.0 ? 10.0 : 3.0;
  } else {
    gl_Position = vec4(0.0);
    gl_FrontColor = vec4(0.0);
    gl_PointSize = 0.0;
  }
}
