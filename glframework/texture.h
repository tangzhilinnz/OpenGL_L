#pragma once
#include"core.h"

class Texture {
public:
	Texture() = default;
	~Texture();

	void initTexture(const char* path, unsigned int unit, bool mipmap = false);
	void bind() const;

	int getWidth()const { return mWidth; }
	int getHeight()const { return mHeight; }

private:
	GLuint mTexture{ 0 };
	int mWidth{ 0 };
	int mHeight{ 0 };
	unsigned int mUnit{ 0 };
};