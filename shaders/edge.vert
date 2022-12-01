#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(void) {
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}
