#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 outUV;

uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;

void main() {
  vec4 _pos = _projMatrix * _viewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  // because we are using a cube map, we need to use the position as the UV
  outUV = aPos;
  // we need to set the w component to 1.0 because we are using a perspective
  // projection
  gl_Position = _pos.xyww;
}