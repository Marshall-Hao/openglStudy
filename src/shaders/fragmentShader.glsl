#version 330 core
out vec4 FragColor;

in vec2 outUV;

// default is sampler2D ~ 0
uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, outUV);
}