#version 460 core
out vec4 FragColor;

in vec2 textureCoord;

uniform sampler2D screenTexSampler;

void main()
{
  FragColor = texture(screenTexSampler, textureCoord);
}