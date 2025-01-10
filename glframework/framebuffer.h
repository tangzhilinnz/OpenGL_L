#pragma once

#include "core.h"
#include "../wrapper/checkError.h"

#include <vector>

class Framebuffer
{
public:
	Framebuffer() = default;
	Framebuffer(unsigned int width, unsigned int height, GLuint unit);
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;
	virtual ~Framebuffer();

	static Framebuffer* createFramebuffer(unsigned int width = 800, unsigned int height = 600, GLuint unit = 0);

	void resizeFramebuffer(unsigned int newWidth, unsigned int newHeight);

	// Uniform method to destroy all instances
	static void destroyAllInstances();
	static void resizeAllInstances(unsigned int newWidth, unsigned int newHeight);
	static const std::vector<Framebuffer*>& getInstances() { return bookmark; }

	GLuint getFBO() const { return mFBO; }
	GLuint getUnit() const { return mUnit; }
	GLuint getColorAttachment() const { return mColorAttachment; }
	GLuint getDepthStencilAttachment() const { return mDepthStencilAttachment; }

	unsigned int getWidth() const { return mWidth; }
	unsigned int getHeight() const { return mHeight; }

	void bindColorAttachment() const
	{
		//先切换纹理单元，然后绑定texture对象
		GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, mColorAttachment));
	}

private:
	GLuint mFBO{ 0 };
	GLuint mColorAttachment{ 0 };
	GLuint mUnit{ 0 };
	GLuint mDepthStencilAttachment{ 0 };

	unsigned int mWidth{ 0 };
	unsigned int mHeight{ 0 };

	// Static bookmark to store instances
	static std::vector<Framebuffer*> bookmark;

};