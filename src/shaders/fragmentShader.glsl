#version 330 core
out vec4 FragColor;

in vec2 outUV;

// default is sampler2D ~ 0
uniform sampler2D tex;

void main()
{   
    vec4  sunlight = vec4(1.0,1.0,1.0, 1.0) * 0.3;
    FragColor = texture(tex, outUV) * sunlight;
}