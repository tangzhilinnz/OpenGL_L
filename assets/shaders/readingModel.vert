#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat3 normalMatrix;

void main()
{
    // ������Ķ���λ�ã�ת��Ϊ������꣨3ά-4ά��
    vec4 transformation = vec4(aPos, 1.0f);

    //��һ���м����TransformPosition�����ڼ�����λλ����modelMatrix��˵��м���
    transformation = modelMatrix * transformation;

    //���㵱ǰ�����worldPosition������������FragmentShader
    worldPosition = transformation.xyz;

    gl_Position = projectionMatrix * viewMatrix * transformation;

    uv = aUV;
    normal = normalMatrix * aNormal;
}
