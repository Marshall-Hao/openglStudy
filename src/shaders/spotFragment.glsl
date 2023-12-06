// #type fragment
#version 330 core
out vec4 FragColor;

in vec2 outUV;
in vec3 outFragPos;
in vec3 outNormal;

// default is sampler2D ~ 0
uniform sampler2D tex;
uniform vec3 _viewPos;

struct Material {
  // material color parameter
  sampler2D m_diffuse;
  sampler2D m_specular;
  // material shininess
  float m_shininess;
};

uniform Material myMaterial;
struct Light {
  // light position parameter
  vec3 m_position;
  vec3 m_direction;

  // light cut off parameter
  float m_cutOff;
  float m_outerCutOff;

  // light color parameter
  vec3 m_ambient;
  vec3 m_diffuse;
  vec3 m_specular;

  // attenuation
  float m_constant;
  float m_linear;
  float m_quadratic;
};

uniform Light myLight;

void main() {
  // light line direction , for the point on the surface and the light position
  vec3 _lightDir = normalize(myLight.m_position - outFragPos);
  // myLight.m_direction is the actual direction of the light
  vec3 _spotDir = normalize(myLight.m_direction);
  float _cosTheta = dot(_lightDir, -_spotDir);

  // epsilon is the angle between the light cut off and the light outter cut off
  float _epsilon = myLight.m_cutOff - myLight.m_outerCutOff;
  // intensity is the light intensity corresponding to the angle and epsilon
  float intensity =
      clamp((_cosTheta - myLight.m_outerCutOff) / _epsilon, 0.0, 1.0);

  // attenuation
  float attenuation =
      1.0 /
      (myLight.m_constant +
       myLight.m_linear * length(myLight.m_position - outFragPos) +
       myLight.m_quadratic * pow(length(myLight.m_position - outFragPos), 2.0));

  // the theta less than phi , the light is on
  // ambient
  vec3 _ambient =
      myLight.m_ambient * vec3(texture(myMaterial.m_diffuse, outUV));
  // diffuse
  // normal line direction
  vec3 _normal = normalize(outNormal);

  // cos theta between normal and light
  float _diff = max(dot(_normal, _lightDir), 0.0);
  // get the diffuse light ,on the x direction costheta
  vec3 _diffuse = myLight.m_diffuse * _diff *
                  vec3(texture(myMaterial.m_diffuse, outUV)) * intensity;

  // reflect line direction
  float _shininess = 32.0;
  vec3 _viewDir = normalize(_viewPos - outFragPos);
  // reflect line direction
  vec3 _reflectDir = reflect(-_lightDir, _normal);
  // cos alpha between reflect and view
  float _spec =
      pow(max(dot(_viewDir, _reflectDir), 0.0), myMaterial.m_shininess);
  vec3 _specular = myLight.m_specular * _spec *
                   vec3(texture(myMaterial.m_specular, outUV)) * intensity;

  vec3 result = _ambient + _diffuse + _specular;
  FragColor = texture(tex, outUV) * vec4(result, 1.0) * attenuation;
}