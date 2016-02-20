#version 120

uniform vec4 transform;

attribute vec2 position;

void main () {
  gl_Position = vec4(transform.zw * (position + transform.xy), 0.0, 1.0);
}
