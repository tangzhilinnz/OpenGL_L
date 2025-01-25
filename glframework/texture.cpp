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
	//1 检查是否缓存过本路径对应的纹理对象
	auto iter = mTextureCache.find(path);
	if (iter != mTextureCache.end())
	{
		return iter->second;
	}

	//2 如果本路径对应的texture没有生成过，则重新生成
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
	//1 检查是否缓存过本路径对应的纹理对象
	auto iter = mTextureCache.find(path);
	if (iter != mTextureCache.end())
	{
		return iter->second;
	}

	//2 如果本路径对应的texture没有生成过，则重新生成
	auto texture = new Texture();
	texture->initTexture(unit, dataIn, widthIn, heightIn);
	mTextureCache[std::string(path)] = texture;
	texture->mCacheName = std::string(path);

	return texture;
}

std::string Texture::genCacheNameForCubeMap(const char* filePath)
{
	std::string path(filePath);
	size_t lastSlashPos = path.find_last_of("/\\");

	// No directory component found, return "CUBE_MAP"
	if (lastSlashPos == std::string::npos)
	{
		return "CUBE_MAP";
	}

	std::string directoryPath = path.substr(0, lastSlashPos);
	directoryPath += "/CUBE_MAP";

	return directoryPath;
}

Texture* Texture::createCubeMapTexture(const char** paths, unsigned int unit)
{
	//1 检查是否缓存过本路径对应的纹理对象
	std::string cacheName = Texture::genCacheNameForCubeMap(paths[0]);
	auto iter = mTextureCache.find(cacheName);
	if (iter != mTextureCache.end())
	{
		return iter->second;
	}

	//2 如果本路径对应的texture没有生成过，则重新生成
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

	//1 stbImage 读取图片
	int channels;

	//--反转y轴
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path, &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	//2 生成纹理并且激活单元绑定
	GL_CALL(glGenTextures(1, &mTexture));
	//--激活纹理单元--
	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	//--绑定纹理对象--
	GL_CALL(glBindTexture(GL_TEXTURE_2D, mTexture));

	//3 传输纹理数据,开辟显存
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA,
		         GL_UNSIGNED_BYTE, data));

	if (this->mMipmap)
	{
		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	}

	//***释放数据
	stbi_image_free(data);

	//4 设置纹理的过滤方式
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mMagFilter));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->mMinFilter));

	//5 设置纹理的包裹方式
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->mWrapS));//u
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->mWrapT));//v

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::initTexture(unsigned int unit, unsigned char* dataIn,
	                      uint32_t widthIn, uint32_t heightIn)
{
	this->cleanup();

	mUnit = unit;

	//1 stbImage 读取图片
	int channels;

	//--反转y轴
	stbi_set_flip_vertically_on_load(true);

	//计算整张图片的大小
	//Assimp规定：如果内嵌纹理是png或者jpg压缩格式的话，height = 0，width就代
	//表了图片大小
	uint32_t dataInSize = 0;
	if (!heightIn)
	{
		dataInSize = widthIn;
	}
	else {
		//如果内嵌图片不是压缩格式，height = 正常高度，width = 正常宽度
		//偷懒：统一认为数据格式都是RGBA
		dataInSize = widthIn * heightIn * 4;//字节为单位
	}

	unsigned char* data = stbi_load_from_memory(
		dataIn, dataInSize, &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	//2 生成纹理并且激活单元绑定
	GL_CALL(glGenTextures(1, &mTexture));
	//--激活纹理单元--
	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	//--绑定纹理对象--
	GL_CALL(glBindTexture(GL_TEXTURE_2D, mTexture));

	//3 传输纹理数据,开辟显存
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA,
		         GL_UNSIGNED_BYTE, data));

	if (this->mMipmap)
	{
		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	}

	//***释放数据
	stbi_image_free(data);

	//4 设置纹理的过滤方式
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mMagFilter));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->mMinFilter));

	//5 设置纹理的包裹方式
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->mWrapS));//u
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->mWrapT));//v

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

// paths:右左上下后前(+x -x +y -y +z -z)
void Texture::initTexture(const char** paths, unsigned int unit)
{
	this->cleanup();

	mUnit = unit;
	mTextureTarget = GL_TEXTURE_CUBE_MAP;

	//cubemap不需要反转y轴
	stbi_set_flip_vertically_on_load(false);

	//1 创建CubeMap对象
	GL_CALL(glGenTextures(1, &mTexture));
	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture));

	//2 循环读取六张贴图，并且放置到cubemap的六个GPU空间内
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
			std::cerr << "Error: CubeMap Texture failed to load at path　－"
				      << paths[i] << std::endl;
			assert(false);
			stbi_image_free(data);
		}
	}

	if (this->mMipmap)
	{
		GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
	}

	//3 设置纹理参数
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, this->mMagFilter));
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, this->mMinFilter));
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_S, this->mWrapS)); //u
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_T, this->mWrapT)); //v
	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_R, this->mWrapR)); //w

	GL_CALL(glBindTexture(mTextureTarget, 0));
}


void Texture::enableMipmap()
{
	this->mMipmap = true;

	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(mTextureTarget, mTexture));

	GL_CALL(glGenerateMipmap(mTextureTarget));

	GL_CALL(glBindTexture(mTextureTarget, 0));
}

void Texture::diableMipmap()
{
	this->mMipmap = false;

	GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
	GL_CALL(glBindTexture(mTextureTarget, mTexture));

	GL_CALL(glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

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