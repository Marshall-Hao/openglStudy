#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aNormal;

out vec3 _normal;
uniform mat4 _modelMatrix;
uniform mat4 _viewMatrix;

void main() {
  gl_Position = _viewMatrix * _modelMatrix * vec4(aPos, 1.0);
  mat3 _normalMatrix = mat3(transpose(inverse(_viewMatrix * _modelMatrix)));
  _normal = _normalMatrix * aNormal;
}