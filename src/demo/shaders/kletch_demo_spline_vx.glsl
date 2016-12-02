#version 120

uniform mat3 matrix;
uniform vec3 even_color;
uniform vec3 odd_color;

attribute vec3 position_parity;

varying vec3 frag_color;

void main () {
  vec3 m_position = matrix * vec3(position_parity.xy, 1.0);
  gl_Position = vec4(m_position.xy / m_position.z, 0.0, 1.0);
  frag_color = position_parity[2] < 0.5 ? odd_color : even_color;
}
