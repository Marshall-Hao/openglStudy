#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 outPos;
out vec3 outNormal;

uniform mat4 _modelMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;

void main() {
  gl_Position = _projMatrix * _viewMatrix * _modelMatrix *
                vec4(aPos.x, aPos.y, aPos.z, 1.0);
  // get the normal after the model matrix is applied
  outNormal = mat3(transpose(inverse(_modelMatrix))) * aNormal;
  outPos = vec3(_modelMatrix * vec4(aPos, 1.0));

  // optional

  // gl_PointSize = 10.0;
  // gl_VertexID;
}