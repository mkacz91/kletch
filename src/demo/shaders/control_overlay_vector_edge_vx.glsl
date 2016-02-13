#version 120

uniform vec4 transform;

attribute vec2 position;

void main () {
  gl_Position = vec4(mix(transform.xy, transform.zw, position), 0, 1);
}
