#version 120

uniform vec4 transform;

attribute vec2 position;

void main () {
  gl_Position = vec4(
    transform.x + transform.z * position.x,
    transform.y + transform.w * position.y,
    0.0,
    1.0
  );
}
