#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

uniform mat4 _modelMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _projectionMatrix;
void main() {
  gl_Position = _projectionMatrix * _viewMatrix * _modelMatrix *
                vec4(aPos.x, aPos.y, aPos.z, 1.0);
}