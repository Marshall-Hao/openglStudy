#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 3) in mat4 aInsModelMatrix;
out vec2 outUV;
out vec3 outFragPos;

uniform mat4 _modelMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;

void main() {
  gl_Position = _projMatrix * _viewMatrix * aInsModelMatrix *
                vec4(aPos.x, aPos.y, aPos.z, 1.0);
  outUV = aUV;
  outFragPos = vec3(aInsModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0));
}