#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 uv;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    vec4 transformPosition = vec4(aPos, 1.0);

	gl_Position = projectionMatrix * viewMatrix * transformPosition;

	gl_Position = gl_Position.xyww;
	
	uv = aUV;
}