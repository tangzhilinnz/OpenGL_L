#version 460 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexSampler;

uniform float texWidth;
uniform float texHeight;

//��ɫ����
vec3 colorInvert(vec3 color)
{
	vec3 invertColor = vec3(1.0f) - color;
	return invertColor;
}

//�������۶�����ɫ�������У�����ɫ�����У�Ϊ������ȷ
//����Ϊÿ����ɫͨ��������Ȩ��ֵ������ɫСһЩ����ɫ��һЩ
vec3 grayCorrect(vec3 color)
{
	float avg = color.r * 0.2126f + color.g * 0.7152f +color.b * 0.0722f;
	return vec3(avg);
}

vec3 blur()
{
	float du = 1.0f / texWidth;
	float dv = 1.0f / texHeight;

	//ƫ��ֵ����
	vec2 offsets[9]=vec2[] (
		vec2(-du, dv),//����
		vec2(0.0f, dv),//����
		vec2(du, dv),//����
		vec2(-du, 0.0f),//��
		vec2(0.0f, 0.0f),//��
		vec2(du, 0.0f),//��
		vec2(-du, -dv),//����
		vec2(0.0f, -dv),//����
		vec2(du, -dv)//����
	);

	//�����
	float kernel[9] = float[] (
		1.0f, 2.0f, 1.0f,
		2.0f, 4.0f, 2.0f,
		1.0f, 2.0f, 1.0f
	);

	//��Ȩ���
	vec3 sumColor = vec3(0.0f);

	for(int i = 0; i < 9; i++)
	{
		vec3 sampleColor = texture(screenTexSampler, uv + offsets[i]).rgb;
		sumColor += sampleColor * kernel[i];
	}

	sumColor /= 16.0f;

	return sumColor;
}

void main()
{
	vec3 color = colorInvert(texture(screenTexSampler, uv).rgb);
	//vec3 color = grayCorrect(texture(screenTexSampler, uv).rgb);
    //vec3 color = texture(screenTexSampler, uv).rgb;
	//vec3 color = blur();

	//FragColor = vec4(texture(screenTexSampler, uv).rgb, 1.0);
	FragColor = vec4(color, 1.0f);
}