#pragma once

#include "core.h"
#include "../wrapper/checkError.h"
#include "tools/capabilities.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <memory>

enum class AttachmentType
{
	COLOR_ATTM,
	DEPTH_ATTM,
	STENCIL_ATTM,
	DEPTH_STENCIL_ATTM
};

enum class BufferType
{
	TEXTURE_2D,
	RENDER_BUFFER
};

class AttachmentGL
{
	friend class FboGL;
public:
	AttachmentGL() = default;
	AttachmentGL(const AttachmentGL&) = delete;
	AttachmentGL& operator=(const AttachmentGL&) = delete;
	virtual ~AttachmentGL();

	static AttachmentGL* create();

	// Uniform method to destroy all instances
	static void destroyAllInstances();
	static const std::vector<AttachmentGL*>& getInstances() { return bookmark; }

	static void resizeAllInstances(int _newWidth, int _newHeight);

public:
	void genBuffer(AttachmentType _attmType, BufferType _bufferType, int _width,
		int _height, GLenum _internalFormat, GLenum _format, GLenum _type);

	void resizeBuffer(int _newWidth, int _newHeight);

	void bindAttmTex(unsigned int _unit) const
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0 + _unit));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, attmID));
	}

private:
	GLuint attmID{ 0 };
	AttachmentType attmType { AttachmentType::COLOR_ATTM };
	BufferType bufferType{ BufferType::TEXTURE_2D };

	GLenum internalFormat{ 0 };
	GLenum format{ 0 };
	GLenum type{ 0 };

	int width{ 800 };
	int height{ 600 };

private:
	static std::vector<AttachmentGL*> bookmark;

private:
	void genRenderBuffer()
	{
		GL_CALL(glGenRenderbuffers(1, &this->attmID));
		GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, this->attmID));

		switch (this->attmType)
		{
		case AttachmentType::COLOR_ATTM:
		{
			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER,
				this->internalFormat, this->width, this->height));
			break;
		}
		case AttachmentType::DEPTH_ATTM:
		{
			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
				this->width, this->height));
			break;
		}
		case AttachmentType::STENCIL_ATTM:
		{
			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8,
				this->width, this->height));
			break;
		}
		case AttachmentType::DEPTH_STENCIL_ATTM:
		{
			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
				this->width, this->height));
			break;
		}
		default:
		{
			std::cerr << "Unrecognized Attachment Type!" << std::endl;
			assert(false);
		}
		}

		//GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER,
		//	this->internalFormat, this->width, this->height));
		GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}

	void genTextureBuffer()
	{
		GL_CALL(glGenTextures(1, &this->attmID));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, this->attmID));

		switch (this->attmType)
		{
		case AttachmentType::COLOR_ATTM:
		{
			GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat,
				this->width, this->height, 0, this->format, this->type, nullptr));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			break;
		}
		case AttachmentType::DEPTH_ATTM:
		{
			GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->width,
				this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			break;
		}
		case AttachmentType::STENCIL_ATTM:
		{
			GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8, this->width,
				this->height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			break;
		}
		case AttachmentType::DEPTH_STENCIL_ATTM:
		{
			GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->width,
				this->height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			break;
		}
		default:
		{
			std::cerr << "Unrecognized Attachment Type!" << std::endl;
			assert(false);
		}
		}

		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}
};

class FboGL
{
public:
	FboGL() = default;
	FboGL(const FboGL&) = delete;
	FboGL& operator=(const FboGL&) = delete;
	virtual ~FboGL();

	static FboGL* create();

	// Uniform method to destroy all instances
	static void destroyAllInstances();
	static const std::vector<FboGL*>& getInstances() { return bookmark; }
	static void reBindAllInstances();


public:
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
	void genFBO();
	void bindAttm(AttachmentGL* attm);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

private:
	GLuint fboID{ 0 };
	std::vector<GLenum> drawBuffers;
	std::vector<AttachmentGL*> mAttms;

private:
	// Static bookmark to store instances
	static std::vector<FboGL*> bookmark;

public:
	void begin() const
	{
		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, this->fboID));
	}

	void end() const
	{
		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

private:
	void rebindAllAttms();

	void framebufferSupport();

private:
	void bindRenderbufferAttm(AttachmentGL* attm)
	{
		switch (attm->attmType)
		{
		case AttachmentType::COLOR_ATTM:
		{
			GLenum COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0 + this->drawBuffers.size();

			GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER,
				COLOR_ATTACHMENT, GL_RENDERBUFFER,
				attm->attmID));
			this->drawBuffers.push_back(COLOR_ATTACHMENT);
			break;
		}
		case AttachmentType::DEPTH_ATTM:
		{
			GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, attm->attmID));
			break;
		}
		case AttachmentType::STENCIL_ATTM:
		{
			GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER, attm->attmID));
			break;
		}
		case AttachmentType::DEPTH_STENCIL_ATTM:
		{
			GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER, attm->attmID));
			break;
		}
		default:
		{
			std::cerr << "Unrecognized Attachment Type!" << std::endl;
			assert(false);
		}
		}
	}

	void bindTextureAttm(AttachmentGL* attm)
	{
		switch (attm->attmType)
		{
		case AttachmentType::COLOR_ATTM:
		{
			GLenum COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0 + this->drawBuffers.size();
			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER,
				COLOR_ATTACHMENT, GL_TEXTURE_2D,
				attm->attmID, 0));
			this->drawBuffers.push_back(COLOR_ATTACHMENT);
			break;
		}
		case AttachmentType::DEPTH_ATTM:
		{
			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_TEXTURE_2D, attm->attmID, 0));
			break;
		}
		case AttachmentType::STENCIL_ATTM:
		{
			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
				GL_TEXTURE_2D, attm->attmID, 0));
			break;
		}
		case AttachmentType::DEPTH_STENCIL_ATTM:
		{
			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
				GL_TEXTURE_2D, attm->attmID, 0));
			break;
		}
		default:
		{
			std::cerr << "Unrecognized Attachment Type!" << std::endl;
			assert(false);
		}
		}
	}

	void checkBuffer()
	{
		GLenum status;
		GL_CALL(status=glCheckFramebufferStatus(GL_FRAMEBUFFER));

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
				       << std::endl;

			switch (status) {
			case GL_FRAMEBUFFER_UNDEFINED:
				std::cerr << "GL_FRAMEBUFFER_UNDEFINED: The default framebuffer does not exist." << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: One or more framebuffer attachment points are incomplete." << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: The framebuffer does not have at least one image attached to it." << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi." << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: GL_READ_BUFFER is set to an attachment point that has no image attached." << std::endl;
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED: The combination of internal formats of the attached images violates an implementation-dependent restriction." << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: The number of samples for all attachments is not the same." << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: Not all attachment layers are consistent, or some attachment point does not have layers." << std::endl;
				break;
			default:
				std::cerr << "Unknown error status: 0x" << std::hex << status << std::endl;
				break;
			}

			assert(false);
		}
	}
};