#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
uniform sampler2D sampler;

void main()
{
    vec3 normalN = normalize(normal);
    FragColor = vec4((normalN + 1.0f) * 0.5f, 1.0f);
}