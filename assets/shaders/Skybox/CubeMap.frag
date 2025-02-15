#version 460 core
out vec4 FragColor;

in vec3 uvw;

//uniform sampler2D diffuse;
uniform samplerCube mapSampler;

void main()
{
	FragColor = texture(mapSampler, uvw);
}