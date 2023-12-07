#version 330 core
out vec4 FragColor;

in vec2 outUV;
in vec3 outFragPos;
in vec3 outNormal;

// default is sampler2D ~ 0
uniform vec3 _viewPos;

struct Material {
  sampler2D m_diffuse;
  sampler2D m_specular;

  float m_shininess;
};

uniform Material myMaterial;
struct DirLight {
  vec3 m_direction;
  vec3 m_ambient;
  vec3 m_diffuse;
  vec3 m_specular;
};

struct PointLight {
  vec3 m_position;

  vec3 m_ambient;
  vec3 m_diffuse;
  vec3 m_specular;

  float m_constant;
  float m_linear;
  float m_quadratic;
};

struct SpotLight {
  vec3 m_position;
  vec3 m_direction;
  float m_cutOff;
  float m_outerCutOff;

  vec3 m_ambient;
  vec3 m_diffuse;
  vec3 m_specular;

  float m_constant;
  float m_linear;
  float m_quadratic;
};

// parallel light
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
  vec3 _lightDir = normalize(light.m_direction);
  // ambient
  vec3 _ambient = light.m_ambient * vec3(texture(myMaterial.m_diffuse, outUV));
  // diffuse
  float _diff = max(dot(normal, -_lightDir), 0.0f);
  vec3 _diffuse =
      light.m_diffuse * _diff * vec3(texture(myMaterial.m_diffuse, outUV));
  // specular
  vec3 _reflectDir = reflect(_lightDir, normal);
  float _spec =
      pow(max(dot(_reflectDir, viewDir), 0.0f), myMaterial.m_shininess);
  vec3 _specular =
      light.m_specular * _spec * vec3(texture(myMaterial.m_specular, outUV));

  return _ambient + _diffuse + _specular;
}

// point light
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
  // direction is from the light to the fragment(points lerp value) , tail to
  // head b -a = ab a-> b
  vec3 _lightDir = normalize(fragPos - light.m_position);
  // ambient
  vec3 _ambient = light.m_ambient * vec3(texture(myMaterial.m_diffuse, outUV));
  // diffuse
  float _diff = max(dot(normal, -_lightDir), 0.0f);
  vec3 _diffuse =
      light.m_diffuse * _diff * vec3(texture(myMaterial.m_diffuse, outUV));

  // specular
  vec3 _reflectDir = reflect(_lightDir, normal);
  float _spec =
      pow(max(dot(_reflectDir, viewDir), 0.0f), myMaterial.m_shininess);
  vec3 _specular =
      light.m_specular * _spec * vec3(texture(myMaterial.m_specular, outUV));

  // attenuation
  float _distance = length(fragPos - light.m_position);
  float _attenuation = 1.0f / (light.m_constant + light.m_linear * _distance +
                               light.m_quadratic * _distance * _distance);

  return _attenuation * (_ambient + _diffuse + _specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
  // direction is from the light to the fragment(points lerp value) , tail to
  // head b -a = ab a-> b
  vec3 _lightDir = normalize(fragPos - light.m_position);
  vec3 _spotDir = normalize(light.m_direction);
  float _cosTheta = dot(_lightDir, _spotDir);

  // eplison
  float _epsilon = light.m_cutOff - light.m_outerCutOff;

  // intensity
  float intersity =
      clamp((_cosTheta - light.m_outerCutOff) / _epsilon, 0.0f, 1.0f);

  // attenuation
  float attenuation =
      1.0 /
      (light.m_constant +
       light.m_linear * length(light.m_position - outFragPos) +
       light.m_quadratic * pow(length(light.m_position - outFragPos), 2.0));

  // ambient
  vec3 _ambient = light.m_ambient * vec3(texture(myMaterial.m_diffuse, outUV));
  // diffuse
  float _diff = max(dot(normal, -_lightDir), 0.0f);
  vec3 _diffuse =
      light.m_diffuse * _diff * vec3(texture(myMaterial.m_diffuse, outUV));

  // specular
  vec3 _reflectDir = reflect(_lightDir, normal);
  float _spec =
      pow(max(dot(_reflectDir, viewDir), 0.0f), myMaterial.m_shininess);
  vec3 _specular =
      light.m_specular * _spec * vec3(texture(myMaterial.m_specular, outUV));

  // attenuation
  float _distance = length(fragPos - light.m_position);
  float _attenuation = 1.0f / (light.m_constant + light.m_linear * _distance +
                               light.m_quadratic * _distance * _distance);

  return _attenuation *
         (_ambient + intersity * _diffuse + intersity * _specular);
}

#define MAX_POINT_LIGHTS 4
uniform DirLight myDirLight;
uniform PointLight myPointLight[MAX_POINT_LIGHTS];
uniform SpotLight mySpotLight;

void main() {
  vec3 _normal = normalize(outNormal);
  // toward to the viewer (camera)
  vec3 _viewDir = normalize(_viewPos - outFragPos);

  vec3 result = calcDirLight(myDirLight, _normal, _viewDir);

  for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
    result += calcPointLight(myPointLight[i], _normal, _viewDir, outFragPos);
  }

  result += calcSpotLight(mySpotLight, _normal, _viewDir, outFragPos);

  FragColor = vec4(result, 1.0);
}