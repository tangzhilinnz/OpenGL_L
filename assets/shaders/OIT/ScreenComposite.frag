#version 460 core

// shader outputs
layout(location = 0) out vec4 frag;

// color accumulation buffer
uniform sampler2D accum;

// revealage threshold buffer
uniform sampler2D reveal;

// epsilon number
const float EPSILON = 0.00001f;

// calculate floating point numbers equality accurately
bool isApproximatelyEqual(float a, float b)
{
  return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * EPSILON;
}

// get the max value between three values
float max3(vec3 v) { return max(max(v.x, v.y), v.z); }

void main()
{
  // fragment coordination
  // coords 是屏幕坐标，范围为 ([0,x), [0,y))
  ivec2 coords = ivec2(gl_FragCoord.xy);

  // 输入为 texture, coord (整数离散坐标，表示 texture 的像素大小), mipmap 层级
  // Fetches the revealage value (red channel) from the reveal texture.
  // Since the texelFetch function always returns a vec4, the .r accessor is
  // used to extract the red channel (the only meaningful channel in a GL_R8 texture),
  // ignoring the default values of the other channels.
  float revealage = texelFetch(reveal, coords, 0).r;

  // If revealage is approximately 1.0 (fully transparent), the fragment is
  // discarded (no further processing).
  if (isApproximatelyEqual(revealage, 1.0f))
  {
      discard;
  }

  // Fetches the accumulated color and alpha value from the accum texture.
  // Checks for overflow (infinity values) in the RGB components. If found,
  // it resets the RGB to the alpha value as a fallback.
  vec4 accumulation = texelFetch(accum, coords, 0);

  // suppress overflow
  if (isinf(max3(abs(accumulation.rgb))))
  {
      accumulation.rgb = vec3(accumulation.a);
  }

  // prevent floating point precision bug
  vec3 average_color = accumulation.rgb / max(accumulation.a, EPSILON);

  // blend pixels
  frag = vec4(average_color, 1.0f - revealage);
}