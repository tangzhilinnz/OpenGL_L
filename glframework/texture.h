#pragma once
#include"core.h"
#include "../wrapper/checkError.h"

#include <map>

//使用C++的枚举类型

class Texture {
public:
	static Texture* createTexture(const char* path, unsigned int unit);
	static Texture* createTextureFromMemory(
		const char* path,
		unsigned int unit,
		unsigned char* dataIn,
		uint32_t widthIn,
		uint32_t heightIn);
	static Texture* createCubeMapTexture(const char** paths, unsigned int unit);

	static void clearCache();

	Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	virtual ~Texture();

	void initTexture(const char* path, unsigned int unit);
	void initTexture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn,
		             uint32_t heightIn);
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

	unsigned int getTextureTarget() const { return mTextureTarget; }

private:
	void cleanup();

public:
	void enableMipmap();
	void diableMipmap();

	void setMagFilter(GLint magFilter);
	void setMinFilter(GLint minFilter);

	void setWrapS(GLint wrapS);
	void setWrapT(GLint wrapT);
	void setWrapR(GLint wrapR);

private:
	GLuint mTexture{ 0 };
	int mWidth{ 0 };
	int mHeight{ 0 };
	unsigned int mUnit{ 0 };
	unsigned int mTextureTarget{ GL_TEXTURE_2D };

	bool mMipmap{ true };
	GLint mMagFilter{ GL_NEAREST };
	GLint mMinFilter{ GL_NEAREST_MIPMAP_LINEAR };
	GLint mWrapS{ GL_REPEAT };
	GLint mWrapT{ GL_REPEAT };
	GLint mWrapR{ GL_REPEAT };

	std::string mCacheName{ "" };

	//注意：静态！！属于类的不属于某个对象
	static std::map<std::string, Texture*> mTextureCache;

private:

	static std::string genCacheNameForCubeMap(const char* filePath);
};