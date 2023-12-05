#version 330 core
out vec4 FragColor;

in vec2 outUV;

// default is sampler2D ~ 0
uniform sampler2D tex;
uniform vec3 _lightColor;
uniform float _ambientStrength;

void main()
{   
    vec4 ambient = vec4(_lightColor, 1.0) * _ambientStrength;
    FragColor = texture(tex, outUV) * ambient;
}