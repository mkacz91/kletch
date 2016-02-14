#version 120

uniform vec4 transform; // (tx, ty, sx, sy)
uniform vec3 center_r; // (cx, cy, r)
uniform vec2 angle; // (a0, a1)
uniform vec3 color0;
uniform vec3 color1;

attribute float param;

varying vec3 frag_color;

void main () {
  frag_color = mix(color0, color1, param);
  float a = mix(angle[0], angle[1], param);
  vec2 position = center_r.xy + center_r.z * vec2(cos(a), sin(a));
  gl_Position = vec4(transform.zw * (position + transform.xy), 0.0, 1.0);
}
