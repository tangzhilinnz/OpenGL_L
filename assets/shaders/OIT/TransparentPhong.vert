//#version 460 core
//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aNor;
//layout(location = 2) in vec2 aTexCoord;
//
//uniform mat4 modelMatrix;
//uniform mat4 viewMatrix;
//uniform mat4 projectionMatrix;
//
//out vec2 textureCoord;
//
//void main()
//{
//  // 裁剪空间坐标系 (clip space) 中 点的位置
//  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
//  // 纹理坐标
//  textureCoord = aTexCoord;
//}

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
    // 将输入的顶点位置，转化为齐次坐标（3维-4维）
    vec4 transformation = vec4(aPos, 1.0f);

    //做一个中间变量TransformPosition，用于计算四位位置与modelMatrix相乘的中间结果
    transformation = modelMatrix * transformation;

    //计算当前顶点的worldPosition，并且向后传输给FragmentShader
    worldPosition = transformation.xyz;

    gl_Position = projectionMatrix * viewMatrix * transformation;

    uv = aUV;
    normal = normalMatrix * aNormal;
}