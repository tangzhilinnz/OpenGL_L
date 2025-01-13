#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;

uniform float opacity; //Í¸Ã÷¶È

void main()
{
	FragColor = vec4(vec3(1.0f, 1.0f, 1.0f), opacity);
}