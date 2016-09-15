#version 120

uniform mat3 matrix;
uniform vec3 color0;
uniform vec3 color1;

attribute vec3 position_param;

varying vec3 frag_color;

void main () {
  vec3 position = vec3(position_param.xy, 1.0);
  float param = position_param[2];
  frag_color = mix(color0, color1, param);
  position = matrix * position;
  gl_Position = vec4(position.xy / position.z, 0.0, 1.0);
}
