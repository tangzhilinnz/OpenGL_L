#version 460 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D mapSampler;

void main()
{
	FragColor = texture(mapSampler, uv);
}