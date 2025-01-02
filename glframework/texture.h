#pragma once
#include"core.h"
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

	Texture() = default;
	~Texture();

	void initTexture(const char* path, unsigned int unit, bool mipmap = true);
	void initTexture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn,
		             uint32_t heightIn, bool mipmap = true);
	
	void bind() const;
	void setUnit(unsigned int uint) { mUnit = uint; }

	int getWidth()const { return mWidth; }
	int getHeight()const { return mHeight; }

private:
	GLuint mTexture{ 0 };
	int mWidth{ 0 };
	int mHeight{ 0 };
	unsigned int mUnit{ 0 };

	//注意：静态！！属于类的不属于某个对象
	static std::map<std::string, Texture*> mTextureCache;
};