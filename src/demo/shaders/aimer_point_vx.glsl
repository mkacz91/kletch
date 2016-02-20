#version 120

uniform mat3 origin_matrix;
uniform vec2 origin;
uniform vec2 offset_scale;

attribute vec2 offset;

void main () {
  vec3 m_origin = origin_matrix * vec3(origin, 1.0);
  gl_Position = vec4(
    m_origin.xy / m_origin.z + offset * offset_scale,
    0.0,
    1.0
  );
}
