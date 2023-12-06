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

    vec4 result = _ambient + _diffuse;
    FragColor = texture(tex, outUV) * result;
}