#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"

std::map<std::string, Texture*> Texture::mTextureCache{};

void Texture::clearCache()
{
	printf("---- ~Texture clearCache ----\n");

	for (auto& item : mTextureCache)
	{
		// Free the dynamically allocated Texture object
		if (item.second) delete item.second;
	}
	mTextureCache.clear();
}

Texture* Texture::createTexture(const char* path, unsigned int unit)
{
	//1 ����Ƿ񻺴����·����Ӧ���������
	auto iter = mTextureCache.find(path);
	if (iter != mTextureCache.end())
	{
		return iter->second;
	}

	//2 �����·����Ӧ��textureû�����ɹ�������������
	auto texture = new Texture();
	texture->initTexture(path, unit);
	mTextureCache[std::string(path)] = texture;

	return texture;
}

Texture* Texture::createTextureFromMemory(
	const char* path,
	unsigned int unit,
	unsigned char* dataIn,
	uint32_t widthIn,
	uint32_t heightIn
) {
	//1 ����Ƿ񻺴����·����Ӧ���������
	auto iter = mTextureCache.find(path);
	if (iter != mTextureCache.end())
	{
		return iter->second;
	}

	//2 �����·����Ӧ��textureû�����ɹ�������������
	auto texture = new Texture();
	texture->initTexture(unit, dataIn, widthIn, heightIn);
	mTextureCache[std::string(path)] = texture;

	return texture;
}

void Texture::initTexture(const char* path, unsigned int unit, bool mipmap)
{
	mUnit = unit;

	//1 stbImage ��ȡͼƬ
	int channels;

	//--��תy��
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path, &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	//2 ���������Ҽ��Ԫ��
	glGenTextures(1, &mTexture);
	//--��������Ԫ--
	glActiveTexture(GL_TEXTURE0 + mUnit);
	//--���������--
	glBindTexture(GL_TEXTURE_2D, mTexture);

	//3 ������������,�����Դ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA,
		         GL_UNSIGNED_BYTE, data);

	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	//***�ͷ�����
	stbi_image_free(data);

	//4 ��������Ĺ��˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (mipmap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	//5 ��������İ�����ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
}

void Texture::initTexture(unsigned int unit, unsigned char* dataIn,
	                      uint32_t widthIn, uint32_t heightIn, bool mipmap)
{
	mUnit = unit;

	//1 stbImage ��ȡͼƬ
	int channels;

	//--��תy��
	stbi_set_flip_vertically_on_load(true);

	//��������ͼƬ�Ĵ�С
	//Assimp�涨�������Ƕ������png����jpgѹ����ʽ�Ļ���height = 0��width�ʹ�
	//����ͼƬ��С
	uint32_t dataInSize = 0;
	if (!heightIn)
	{
		dataInSize = widthIn;
	}
	else {
		//�����ǶͼƬ����ѹ����ʽ��height = �����߶ȣ�width = �������
		//͵����ͳһ��Ϊ���ݸ�ʽ����RGBA
		dataInSize = widthIn * heightIn * 4;//�ֽ�Ϊ��λ
	}

	unsigned char* data = stbi_load_from_memory(
		dataIn, dataInSize, &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	//2 ���������Ҽ��Ԫ��
	glGenTextures(1, &mTexture);
	//--��������Ԫ--
	glActiveTexture(GL_TEXTURE0 + mUnit);
	//--���������--
	glBindTexture(GL_TEXTURE_2D, mTexture);

	//3 ������������,�����Դ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);

	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	//***�ͷ�����
	stbi_image_free(data);

	//4 ��������Ĺ��˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (mipmap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	//5 ��������İ�����ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
}

Texture::~Texture()
{
	printf("---- ~Texture ----\n");

	if (mTexture != 0)
	{
		glDeleteTextures(1, &mTexture);
	}
}

void Texture::bind() const
{
	//���л�����Ԫ��Ȼ���texture����
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}