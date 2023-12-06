#version 330 core
out vec4 FragColor;

in vec2 outUV;
in vec3 outFragPos;
in vec3 outNormal;

// default is sampler2D ~ 0
uniform sampler2D tex;
uniform vec3 _viewPos;

struct Material
{
 sampler2D m_diffuse;
 sampler2D m_specular;

 float m_shininess;
};

uniform Material myMaterial;
struct Light
{
vec3 m_direction;

 vec3 m_ambient;
 vec3 m_diffuse;
 vec3 m_specular;
};

uniform Light myLight;

void main()
{   
    vec3 _lightDir = normalize(myLight.m_direction);
    // ambient
    vec3 _ambient = myLight.m_ambient * vec3(texture(myMaterial.m_diffuse, outUV)) ;
    // diffuse
    // normal line direction
    vec3 _normal = normalize(outNormal);
    // light line direction
    // vec3 _lightDir = normalize(myLight.m_position - outFragPos);
    // cos theta between normal and light
    float _diff = max(dot(_normal, -_lightDir), 0.0);
    // get the diffuse light ,on the x direction costheta
    vec3 _diffuse = myLight.m_diffuse * _diff * vec3(texture(myMaterial.m_diffuse, outUV));

 
    // reflect line direction
    float _shininess = 32.0;
    vec3 _viewDir = normalize(_viewPos - outFragPos);
    // reflect line direction
    vec3 _reflectDir = reflect(_lightDir, _normal);
    // cos alpha between reflect and view
    float _spec = pow(max(dot(_viewDir, _reflectDir), 0.0), myMaterial.m_shininess);
    vec3 _specular = myLight.m_specular * _spec * vec3(texture(myMaterial.m_specular, outUV));

    vec3 result = _ambient + _diffuse +  _specular;
    FragColor = texture(tex, outUV) * vec4(result,1.0);
}