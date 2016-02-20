#version 120

uniform mat3 matrix;
uniform vec3 center_r; // (cx, cy, r)
uniform vec2 angle; // (a0, a1)
uniform vec3 color0;
uniform vec3 color1;

attribute float param;

varying vec3 frag_color;

void main () {
  frag_color = mix(color0, color1, param);
  float a = mix(angle[0], angle[1], param);
  vec3 position = vec3(center_r.xy + center_r.z * vec2(cos(a), sin(a)), 1.0);
  position = matrix * position;
  gl_Position = vec4(position.xy / position.z, 0.0, 1.0);
}
