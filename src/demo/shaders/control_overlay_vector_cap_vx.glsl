#version 120

uniform vec4 transform; // (tx, ty, scale * sin, scale * cos)

attribute vec2 position;

void main () {
  float s = sin(transform.w), c = cos(transform.w);
  mat2 m = mat2(transform.w, transform.z, -transform.z, transform.w);
  gl_Position = vec4(transform.xy + m * position, 0, 1);
}
