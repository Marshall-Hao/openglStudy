#version 330 core
out vec4 FragColor;

in vec2 outUV;
in vec3 outFragPos;
in vec3 outNormal;

// default is sampler2D ~ 0
uniform sampler2D tex;
uniform vec3 _lightColor;
uniform float _ambientStrength;
uniform vec3 _lightPos;

uniform vec3 _viewPos;

void main()
{   
    // ambient
    vec4 _ambient = vec4(_lightColor, 1.0) * _ambientStrength;
    // diffuse
    // normal line direction
    vec3 _normal = normalize(outNormal);
    // light line direction
    vec3 _lightDir = normalize(_lightPos - outFragPos);
    // cos theta between normal and light
    float _diff = max(dot(_normal, _lightDir), 0.0);
    // get the diffuse light ,on the x direction costheta
    vec4 _diffuse = vec4(_lightColor, 1.0) * _diff;

    // specular
    float _specularStrength = 0.5;
    // reflect line direction
    float _shininess = 32.0;
    vec3 _viewDir = normalize(_viewPos - outFragPos);
    // reflect line direction
    vec3 _reflectDir = reflect(-_lightDir, _normal);
    // cos alpha between reflect and view
    float _spec = pow(max(dot(_viewDir, _reflectDir), 0.0), _shininess);
    vec4 _specular = vec4(_lightColor, 1.0) * _spec * _specularStrength;

    vec4 result = _ambient + _diffuse +  _specular;
    FragColor = texture(tex, outUV) * result;
}