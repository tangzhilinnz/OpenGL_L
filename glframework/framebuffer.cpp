#include "framebuffer.h"
#include <vector>
#include "../wrapper/checkError.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<FboGL*> FboGL::bookmark;

FboGL* FboGL::create()
{
	FboGL* fbo = new FboGL();
	bookmark.push_back(fbo);
	return fbo;
}

void FboGL::destroyAllInstances()
{
	for (FboGL* instance : bookmark)
	{
		if (instance) delete instance;
	}
	bookmark.clear();
}

void FboGL::reBindAllInstances()
{
	for (FboGL* instance : bookmark)
	{
		instance->rebindAllAttms();
	}
}

FboGL:: ~FboGL()
{
	printf("---- ~FboGL ----\n");

	if (this->fboID)
	{
	    GL_CALL(glDeleteFramebuffers(1, &this->fboID));
	}

	for (auto& fbo : FboGL::bookmark)
	{
		if (fbo == this)
		{
			fbo = nullptr;
			break;
		}
	}
}

void FboGL::genFBO()
{
	// Generate a new framebuffer ID
	GL_CALL(glGenFramebuffers(1, &this->fboID));
}

void FboGL::bindAttm(AttachmentGL* attm)
{
	// Avoid duplicate attachment binding
	if (std::find(this->mAttms.begin(), this->mAttms.end(), attm)
		!= this->mAttms.end())
	{
		std::cout << "Duplicate attachment binding: " << std::endl;
		return;
	}

	this->mAttms.push_back(attm);

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, this->fboID));

	if (attm->bufferType == BufferType::RENDER_BUFFER)
	{
		this->bindRenderbufferAttm(attm);
	}
	else
	{
		this->bindTextureAttm(attm);
	}

	GL_CALL(glDrawBuffers(this->drawBuffers.size(), this->drawBuffers.data()));

	this->checkBuffer();

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FboGL::rebindAllAttms()
{
	this->drawBuffers.clear();

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, this->fboID));

	for (size_t i = 0; i < this->mAttms.size(); i++)
	{
		AttachmentGL* attm = mAttms[i];

		if (attm->bufferType == BufferType::RENDER_BUFFER)
		{
			this->bindRenderbufferAttm(attm);
		}
		else
		{
			this->bindTextureAttm(attm);
		}
	}

	GL_CALL(glDrawBuffers(this->drawBuffers.size(), this->drawBuffers.data()));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
			      << std::endl;
		assert(false);
	}

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<AttachmentGL*> AttachmentGL::bookmark;


AttachmentGL* AttachmentGL::create()
{
	AttachmentGL* attm = new AttachmentGL();
	bookmark.push_back(attm);
	return attm;
}

void AttachmentGL::destroyAllInstances()
{
	for (AttachmentGL* instance : AttachmentGL::bookmark)
	{
		if (instance) delete instance;
	}
	bookmark.clear();
}

void AttachmentGL::resizeAllInstances(int _newWidth, int _newHeight)
{
	for (AttachmentGL* instance : bookmark)
	{
		instance->resizeBuffer(_newWidth, _newHeight);
	}
}

AttachmentGL:: ~AttachmentGL()
{
	printf("---- ~AttachmentGL ----\n");

	if (this->attmID)
	{
		if (this->bufferType == BufferType::RENDER_BUFFER)
		{
			GL_CALL(glDeleteRenderbuffers(1, &this->attmID));
		}
		else
		{
			GL_CALL(glDeleteTextures(1, &this->attmID));
		}
	}

	for (auto& attm : bookmark)
	{
		if (attm == this)
		{
			attm = nullptr;
			break;
		}
	}
}

void AttachmentGL::genBuffer(AttachmentType _attmType, BufferType _bufferType,
	int _width, int _height, GLenum _internalFormat, GLenum _format, GLenum _type)
{
	this->attmType = _attmType;
	this->bufferType = _bufferType;
	this->internalFormat = _internalFormat;
	this->format = _format;
	this->type = _type;
	this->width = _width;
	this->height = _height;

	if (this->bufferType == BufferType::RENDER_BUFFER)
	{
		this->genRenderBuffer();
	}
	else
	{
		this->genTextureBuffer();
	}
}

void AttachmentGL::resizeBuffer(int _newWidth, int _newHeight)
{
	this->width = _newWidth;
	this->height = _newHeight;

	if (this->bufferType == BufferType::RENDER_BUFFER)
	{
		GL_CALL(glDeleteRenderbuffers(1, &this->attmID));
		this->attmID = 0;
		this->genRenderBuffer();
	}
	else
	{
		GL_CALL(glDeleteTextures(1, &this->attmID));
		this->attmID = 0;
		this->genTextureBuffer();
	}
}