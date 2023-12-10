#version 330 core
out vec4 FragColor;

in vec3 outNormal;
in vec3 outPos;

uniform vec3 _viewPos;
uniform samplerCube ourTexture;

// in VS_OUT {
//   vec3 outNormal;
//   vec3 outPos;
// } fs_in;

void main() {
  float ratio = 1.0 / 1.52;
  // direction to the fragment from the view
  vec3 I = normalize(outPos - _viewPos);
  // reflect the view direction
  // vec3 R = reflect(I, normalize(outNormal));
  vec3 R = refract(I, normalize(outNormal), ratio);
  FragColor = texture(ourTexture, R);

  // gl_FragCoord.x
  // gl_FrontFacing;
  // gl_FragDepth;
}