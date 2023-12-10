#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 _modelMatrix;

layout(std140) uniform MAT {
  mat4 _projMatrix;
  mat4 _viewMatrix;
};

void main() {
  gl_Position = _projMatrix * _viewMatrix * _modelMatrix *
                vec4(aPos.x, aPos.y, aPos.z, 1.0);
}