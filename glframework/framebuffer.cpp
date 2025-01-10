#include "framebuffer.h"
#include <vector>
#include "../wrapper/checkError.h"

#include <iostream>

std::vector<Framebuffer*> Framebuffer::bookmark;

Framebuffer::Framebuffer(unsigned int width, unsigned int height, GLuint unit)
{
	mWidth = width;
	mHeight = height;
	mUnit = unit;

	//生成fbo对象并且绑定
	GL_CALL(glGenFramebuffers(1, &mFBO));
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));

	//生成颜色附件，并且加入fbo
	GL_CALL(glGenTextures(1, &mColorAttachment));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, mColorAttachment));
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));//u
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));//v
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0));


	//生成depth Stencil附件，加入fbo
	GL_CALL(glGenRenderbuffers(1, &mDepthStencilAttachment));
	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilAttachment));
	GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight));
	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilAttachment));

	//GL_CALL(glGenTextures(1, &mDepthStencilAttachment));
	//GL_CALL(glBindTexture(GL_TEXTURE_2D, mDepthStencilAttachment));
	//GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mWidth, mHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));
	//GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	//GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	//GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	//GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthStencilAttachment, 0));

	//检查当前构建的fbo是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Error:FrameBuffer is not complete!" << std::endl;
		assert(false);
	}

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

Framebuffer::~Framebuffer()
{
	printf("---- ~Framebuffer ----\n");

	if (mFBO)
	{
		GL_CALL(glDeleteFramebuffers(1, &mFBO));
	}
	if (mColorAttachment)
	{
		GL_CALL(glDeleteTextures(1, &mColorAttachment));
	}
	if (mDepthStencilAttachment)
	{
		GL_CALL(glDeleteRenderbuffers(1, &mDepthStencilAttachment));
		//GL_CALL(glDeleteTextures(1, &mDepthStencilAttachment));
	}

	// Set this instance in the static bookmark container to nullptr
	for (auto& fbo : bookmark)
	{
		if (fbo == this)
		{
			fbo = nullptr;
			break;
		}
	}
}

Framebuffer* Framebuffer::createFramebuffer(unsigned int width, unsigned int height, GLuint unit)
{
	Framebuffer* framebuffer = new Framebuffer(width, height, unit);
	bookmark.push_back(framebuffer);
	return framebuffer;
}

void Framebuffer::destroyAllInstances()
{
	for (Framebuffer* instance : bookmark)
	{
		if (instance) delete instance;
	}
	bookmark.clear();
}

void Framebuffer::resizeAllInstances(unsigned int newWidth, unsigned int newHeight)
{
	for (Framebuffer* framework : Framebuffer::bookmark)
	{
		framework->resizeFramebuffer(newWidth, newHeight);
	}
}

void Framebuffer::resizeFramebuffer(unsigned int newWidth, unsigned int newHeight)
{
	mWidth = newWidth;
	mHeight = newHeight;

	// Bind the framebuffer to modify it
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));

	// Delete old attachments
	GL_CALL(glDeleteTextures(1, &mColorAttachment));
	GL_CALL(glDeleteRenderbuffers(1, &mDepthStencilAttachment));

	// Create a new color texture attachment
	GL_CALL(glGenTextures(1, &mColorAttachment));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, mColorAttachment));
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	// Attach the color texture to the framebuffer
	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0));

	// Create a new renderbuffer for depth and stencil attachment
	GL_CALL(glGenRenderbuffers(1, &mDepthStencilAttachment));
	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilAttachment));
	GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight));
	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	// Attach the renderbuffer to the framebuffer
	GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilAttachment));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;
		assert(false);
	}

	// Unbind the framebuffer
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}