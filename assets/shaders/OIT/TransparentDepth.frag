#version 460 core
layout(location = 0) out vec4 accum;
layout(location = 1) out float reveal;

in vec2 uv;
in vec3 normal;

uniform float near;
uniform float far;

uniform float opacity; //Í¸Ã÷¶È

void main()
{
    float Zndc = gl_FragCoord.z * 2.0f - 1.0f;
    float linerDepth = 2.0f * near / (far + near - Zndc * (far - near));

    vec3 finalColor = vec3(linerDepth);

    vec4 color = vec4(finalColor, opacity);

	float weight = clamp(pow(min(1.0, color.a * 10.0) + 0.01, 3.0) * 1e8 *
                           pow(1.0 - gl_FragCoord.z * 0.9, 3.0),
                       1e-2, 3e3);

     accum = vec4(color.rgb * color.a, color.a) * weight;
     reveal = color.a;
}