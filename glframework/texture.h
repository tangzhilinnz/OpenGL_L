#pragma once
#include"core.h"
#include "../wrapper/checkError.h"

#include <map>

class Texture {
public:
	static Texture* createTexture(const char* path, unsigned int unit);
	static Texture* createTextureFromMemory(
		const char* path,
		unsigned int unit,
		unsigned char* dataIn,
		uint32_t widthIn,
		uint32_t heightIn
	);
	static Texture* createCubeMapTexture(const char** paths, unsigned int unit);

	static void clearCache();

	Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	virtual ~Texture();

	void initTexture(const char* path, unsigned int unit, bool mipmap = true);
	void initTexture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn,
		             uint32_t heightIn, bool mipmap = true);
	void initTexture(const char** paths, unsigned int unit);

	void setUnit(unsigned int uint) { mUnit = uint; }

	void bind() const
	{
		//先切换纹理单元，然后绑定texture对象
		GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
		GL_CALL(glBindTexture(mTextureTarget, mTexture));
	}

	int getWidth()const { return mWidth; }
	int getHeight()const { return mHeight; }

	GLuint getTexture() const { return mTexture; }

private:
	GLuint mTexture{ 0 };
	int mWidth{ 0 };
	int mHeight{ 0 };
	unsigned int mUnit{ 0 };
	unsigned int mTextureTarget{ GL_TEXTURE_2D };

	std::string mCacheName{ "" };

	//注意：静态！！属于类的不属于某个对象
	static std::map<std::string, Texture*> mTextureCache;

private:

	static std::string genCacheNameForCubeMap(const char* filePath);
};