#version 120

uniform mat3 matrix;

attribute vec2 position;

void main () {
  vec3 m_position = matrix * vec3(position, 1.0);
  gl_Position = vec4(m_position.xy / m_position.z, 0.0, 1.0);
}
