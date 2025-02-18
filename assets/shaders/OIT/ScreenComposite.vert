#version 460 core
layout(location = 0) in vec2 aPos;

void main()
{
  // 裁剪空间坐标系 (clip space) 中 点的位置
  gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
}