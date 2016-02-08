#version 120

uniform vec4 transform;
attribute vec2 position;

void main () {
  gl_Position = vec4(
    (transform.x + position.x) * transform.z,
    (transform.y + position.y) * transform.w,
    0.0,
    1.0
  );
}
