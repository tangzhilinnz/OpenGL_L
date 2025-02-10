#include "texture.h"
#include <iostream>

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

void Texture::cleanup()
{
	if (mTexture != 0)
	{
		GL_CALL(glDeleteTextures(1, &mTexture));
		mTexture = 0;
	}
}

Texture::~Texture()
{
	printf("---- ~Texture ----\n");

	this->cleanup();

	// Set the corresponding entry in mTextureCache to nullptr
	auto it = mTextureCache.find(mCacheName);
	if (it != mTextureCache.end())
	{
		it->second = nullptr;
	}
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
	texture->mCacheName = std::string(path);

	return texture;
}

Texture* Texture::createTextureFromMemory(
	const char* path,
	unsigned int unit,
	unsigned char* dataIn,
	uint32_t widthIn,
	uint32_t heightIn)
{
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
	texture->mCacheName = std::string(path);

	return texture;
}

std::string Texture::genCacheNameForCubeMap(const char** paths)
{
	std::string firstPath(paths[0]);
	size_t lastSlashPos = firstPath.find_last_of("/\\");

	// If no directory is found, return "CUBE_MAP/"
	std::string directoryPath = (lastSlashPos == std::string::npos) ?
		"CUBE_MAP" : firstPath.substr(0, lastSlashPos);

	directoryPath += "/";

	for (int i = 0; i < 6; i++)
	{
		std::string currentPath(paths[i]);
		size_t lastSlashPos = currentPath.find_last_of("/\\");

		// Extract the filename only (skip slash)
		std::string fileName = (lastSlashPos == std::string::npos) ?
			currentPath : currentPath.substr(lastSlashPos + 1);

		// Append filename with separator
		directoryPath +=  (fileName + "_");
	}

	directoryPath.erase(directoryPath.size() - 1);

	return directoryPath;
}

Texture* Texture::createCubeMapTexture(const char** paths, unsigned int unit)
{
	//1 ����Ƿ񻺴����·����Ӧ���������
	std::string cacheName = Texture::genCacheNameForCubeMap(paths);
	auto iter = mTextureCache.find(cacheName);
	if (iter != mTextureCache.end())
	{
		return iter->second;
	}

	//2 �����·����Ӧ��textureû�����ɹ�������������
	auto texture = new Texture();
	texture->initTexture(paths, unit);
	mTextureCache[cacheName] = texture;
	texture->mCacheName = cacheName;

	return texture;
}

void Texture::initTexture(const char* path, unsigned int unit)
{
	this->cleanup();

	mUnit = unit;

	//1 stbImage ��ȡͼƬ
	int channels;

	//--��תy��
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path, &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	//2 ���������Ҽ��Ԫ��
	GL_CALL(glGenTextures(1, &mTexture));
	//--��������Ԫ--
	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	//--���������--
	GL_CALL(glBindTexture(GL_TEXTURE_2D, mTexture));

	//3 ������������,�����Դ�
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA,
		         GL_UNSIGNED_BYTE, data));

	if (this->mMipmap)
	{
		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	}

	//***�ͷ�����
	stbi_image_free(data);

	//4 ��������Ĺ��˷�ʽ
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mMagFilter));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->mMinFilter));

	//5 ��������İ�����ʽ
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->mWrapS));//u
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->mWrapT));//v

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::initTexture(unsigned int unit, unsigned char* dataIn,
	                      uint32_t widthIn, uint32_t heightIn)
{
	this->cleanup();

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
	GL_CALL(glGenTextures(1, &mTexture));
	//--��������Ԫ--
	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	//--���������--
	GL_CALL(glBindTexture(GL_TEXTURE_2D, mTexture));

	//3 ������������,�����Դ�
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA,
		         GL_UNSIGNED_BYTE, data));

	if (this->mMipmap)
	{
		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	}

	//***�ͷ�����
	stbi_image_free(data);

	//4 ��������Ĺ��˷�ʽ
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mMagFilter));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->mMinFilter));

	//5 ��������İ�����ʽ
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->mWrapS));//u
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->mWrapT));//v

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

// paths:�������º�ǰ(+x -x +y -y +z -z)
void Texture::initTexture(const char** paths, unsigned int unit)
{
	this->cleanup();

	mUnit = unit;
	mTextureTarget = GL_TEXTURE_CUBE_MAP;

	//cubemap����Ҫ��תy��
	stbi_set_flip_vertically_on_load(false);

	//1 ����CubeMap����
	GL_CALL(glGenTextures(1, &mTexture));
	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture));

	//2 ѭ����ȡ������ͼ�����ҷ��õ�cubemap������GPU�ռ���
	int channels;
	int width = 0, height = 0;
	unsigned char* data = nullptr;

	for (int i = 0; i < 6; i++)
	{
		data = stbi_load(paths[i], &width, &height, &channels, STBI_rgb_alpha);
		if (data != nullptr)
		{
			GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height,
				0, GL_RGBA, GL_UNSIGNED_BYTE, data));

			stbi_image_free(data);
		}
		else
		{
			std::cerr << "Error: CubeMap Texture failed to load at path����"
				      << paths[i] << std::endl;
			assert(false);
			stbi_image_free(data);
		}
	}

	if (this->mMipmap)
	{
		GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
	}

	//3 �����������
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, this->mMagFilter));
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, this->mMinFilter));
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_S, this->mWrapS)); //u
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_T, this->mWrapT)); //v
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_R, this->mWrapR)); //w

	// 4 Enable seamless cube map sampling (optional)
	GL_CALL(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));

	GL_CALL(glBindTexture(mTextureTarget, 0));
}

void Texture::setMagFilter(GLint magFilter)
{
	this->mMagFilter = magFilter;

	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(mTextureTarget, mTexture));

	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, mMagFilter));

	GL_CALL(glBindTexture(mTextureTarget, 0));
}

void Texture::setMinFilter(GLint minFilter)
{
	this->mMinFilter = minFilter;

	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(mTextureTarget, mTexture));

	if (!this->mMipmap &&
		(minFilter == GL_NEAREST_MIPMAP_NEAREST ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_LINEAR))
	{
		this->mMipmap = true;
		GL_CALL(glGenerateMipmap(mTextureTarget));
	}
	
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, mMinFilter));

	GL_CALL(glBindTexture(mTextureTarget, 0));
}

void Texture::setWrapS(GLint wrapS)
{
	this->mWrapS = wrapS;

	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(mTextureTarget, mTexture));

	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_S, mWrapS));

	GL_CALL(glBindTexture(mTextureTarget, 0));
}

void Texture::setWrapT(GLint wrapT)
{
	this->mWrapT = wrapT;

	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(mTextureTarget, mTexture));

	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_T, mWrapT));

	GL_CALL(glBindTexture(mTextureTarget, 0));
}

void Texture::setWrapR(GLint wrapR)
{
	this->mWrapR = wrapR;

	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(mTextureTarget, mTexture));

	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_R, wrapR));

	GL_CALL(glBindTexture(mTextureTarget, 0));
}

void Texture::enableAnisotropicFilter(GLfloat anisoSetting)
{
	this->mAnisotropy = true;

	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(mTextureTarget, mTexture));

	// Generate Mipmaps
	GL_CALL(glGenerateMipmap(mTextureTarget));
	// Set Trilinear Filtering
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLfloat maxAniso = 0.0f;
	GL_CALL(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso));

	// Clamp anisoSetting to the supported range
	if (anisoSetting > maxAniso)
	{
		anisoSetting = maxAniso;
	}
	else if (anisoSetting < 1.0f)
	{
		// Minimum valid value
		anisoSetting = 1.0f; 
	}

	GL_CALL(glTexParameterf(mTextureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting));

	GL_CALL(glBindTexture(mTextureTarget, 0));
}

void Texture::disableAnisotropicFilter()
{
	this->mAnisotropy = false;

	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(mTextureTarget, mTexture));


	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, this->mMinFilter));
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, this->mMagFilter));

	// Reset anisotropic filtering to default (1.0 means no AF)
	GL_CALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}