#pragma once

#include "core.h"
#include "../wrapper/checkError.h"

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
};



//class Framebuffer
//{
//public:
//	Framebuffer() = default;
//	Framebuffer(int mWidth, int mHeight);
//	Framebuffer(const Framebuffer&) = delete;
//	Framebuffer& operator=(const Framebuffer&) = delete;
//	virtual ~Framebuffer();
//
//	static Framebuffer* createFramebuffer();
//	static Framebuffer* createFramebuffer(int mWidth, int mHeight);
//
//	// Uniform method to destroy all instances
//	static void destroyAllInstances();
//	static void resizeAllInstances(unsigned int newWidth, unsigned int newHeight);
//	static const std::vector<Framebuffer*>& getInstances() { return bookmark; }
//
//public:
//    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//	void genFBO(std::string _fboName);
//	void genAttm(
//		std::string _attmName, AttachmentType _attmType, BufferType _bufferType,
//		GLenum _internalFormat, GLenum _format, GLenum _type,
//		std::pair<int, int>* _pDims = nullptr);
//	void bindFBOAttm(std::string _fboName, std::string _attmName);
//
//	void updateAttm(std::string _attmName, AttachmentType _attmType, BufferType _bufferType,
//		GLenum _internalFormat, GLenum _format, GLenum _type,
//		std::pair<int, int>* _pDims = nullptr);
//
//	void resizeAttm(std::string _attmName, int newWidth, int newHeight);
//
//	void resizeFramebuffer(int newWidth, int newHeight);
//	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//
//private:
//	std::map<std::string, std::unique_ptr<FBO>> mFBOVec;
//	std::map<std::string, std::unique_ptr<Attachment>> mAttmVec;
//
//	int mWidth{ 800 };
//	int mHeight{ 600 };
//
//private:
//	// Static bookmark to store instances
//	static std::vector<Framebuffer*> bookmark;
//
//public:
//	void beginFBO(std::string _fboName) const
//	{
//		// Use at() to ensure bounds checking
//		auto& fboUptr = this->mFBOVec.at(_fboName);
//		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fboUptr->fboID));
//	}
//
//	void endFBO(std::string fboName) const
//	{
//		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
//	}
//
//	void textureAttmSample(std::string _attmName, unsigned int _unit) const
//	{
//		auto& attmUptr = this->mAttmVec.at(_attmName);
//
//		GL_CALL(glActiveTexture(GL_TEXTURE0 + _unit));
//		GL_CALL(glBindTexture(GL_TEXTURE_2D, attmUptr->attmID));
//	}
//
//private:
//	// The caller of the functions below is responsible for checking the
//	// existence of _attmName and _fboName
//	void genRenderbufferAttm(std::string& _attmName)
//	{
//		GLuint renderbufferID;
//		GL_CALL(glGenRenderbuffers(1, &renderbufferID));
//		GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID));
//
//		auto& attmUptr = this->mAttmVec.at(_attmName);
//
//		attmUptr->attmID = renderbufferID;
//
//		switch (attmUptr->attmType)
//		{
//		case AttachmentType::COLOR_ATTM:
//		{
//			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER,
//				attmUptr->internalFormat, attmUptr->width, attmUptr->height));
//			break;
//		}
//		case AttachmentType::DEPTH_ATTM:
//		{
//			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
//				attmUptr->width, attmUptr->height));
//			break;
//		}
//		case AttachmentType::STENCIL_ATTM:
//		{
//			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8,
//				attmUptr->width, attmUptr->height));
//			break;
//		}
//		case AttachmentType::DEPTH_STENCIL_ATTM:
//		{
//			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
//				attmUptr->width, attmUptr->height));
//			break;
//		}
//		default:
//		{
//			std::cerr << "Unrecognized Attachment Type!" << std::endl;
//			assert(false);
//		}
//		}
//
//		//GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER,
//		//	attmUptr->internalFormat, attmUptr->width, attmUptr->height));
//		GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
//	}
//
//	void genTextureAttm(std::string& _attmName)
//	{
//		GLuint textureID;
//		GL_CALL(glGenTextures(1, &textureID));
//		GL_CALL(glBindTexture(GL_TEXTURE_2D, textureID));
//
//		auto& attmUptr = this->mAttmVec.at(_attmName);
//		attmUptr->attmID = textureID;
//
//		switch (attmUptr->attmType)
//		{
//		case AttachmentType::COLOR_ATTM:
//		{
//			GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, attmUptr->internalFormat,
//				attmUptr->width, attmUptr->height, 0, attmUptr->format,
//				attmUptr->type, nullptr));
//			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
//			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
//			break;
//		}
//		case AttachmentType::DEPTH_ATTM:
//		{
//			GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
//				attmUptr->width, attmUptr->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
//				nullptr));
//			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
//			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
//			break;
//		}
//		case AttachmentType::STENCIL_ATTM:
//		{
//			GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8, attmUptr->width,
//				attmUptr->height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr));
//			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
//			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
//			break;
//		}
//		case AttachmentType::DEPTH_STENCIL_ATTM:
//		{
//			GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, attmUptr->width,
//				attmUptr->height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
//			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
//			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
//			break;
//		}
//		default:
//		{
//			std::cerr << "Unrecognized Attachment Type!" << std::endl;
//			assert(false);
//		}
//		}
//
//		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
//	}
//
//	void bindRenderbufferAttm(std::string& _fboName, std::string& _attmName)
//	{
//		auto& fboUptr = this->mFBOVec.at(_fboName);
//		auto& attmUptr = this->mAttmVec.at(_attmName);
//
//		switch (attmUptr->attmType)
//		{
//		case AttachmentType::COLOR_ATTM:
//		{
//			GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER,
//				GL_COLOR_ATTACHMENT0 + fboUptr->drawBuffers.size(), GL_RENDERBUFFER,
//				attmUptr->attmID));
//			GLenum color_attm = GL_COLOR_ATTACHMENT0 + fboUptr->drawBuffers.size();
//			fboUptr->drawBuffers.push_back(color_attm);
//			break;
//		}
//		case AttachmentType::DEPTH_ATTM:
//		{
//			GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
//				GL_RENDERBUFFER, attmUptr->attmID));
//			break;
//		}
//		case AttachmentType::STENCIL_ATTM:
//		{
//			GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
//				GL_RENDERBUFFER, attmUptr->attmID));
//			break;
//		}
//		case AttachmentType::DEPTH_STENCIL_ATTM:
//		{
//			GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
//				GL_RENDERBUFFER, attmUptr->attmID));
//			break;
//		}
//		default:
//		{
//			std::cerr << "Unrecognized Attachment Type!" << std::endl;
//			assert(false);
//		}
//		}
//	}
//
//	void bindTextureAttm(std::string& _fboName, std::string& _attmName)
//	{
//		auto& fboUptr = this->mFBOVec.at(_fboName);
//		auto& attmUptr = this->mAttmVec.at(_attmName);
//
//		switch (attmUptr->attmType)
//		{
//		case AttachmentType::COLOR_ATTM:
//		{
//			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER,
//				GL_COLOR_ATTACHMENT0 + fboUptr->drawBuffers.size(), GL_TEXTURE_2D,
//				attmUptr->attmID, 0));
//			GLenum color_attm = GL_COLOR_ATTACHMENT0 + fboUptr->drawBuffers.size();
//			fboUptr->drawBuffers.push_back(color_attm);
//			break;
//		}
//		case AttachmentType::DEPTH_ATTM:
//		{
//			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
//				GL_TEXTURE_2D, attmUptr->attmID, 0));
//			break;
//		}
//		case AttachmentType::STENCIL_ATTM:
//		{
//			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
//				GL_TEXTURE_2D, attmUptr->attmID, 0));
//			break;
//		}
//		case AttachmentType::DEPTH_STENCIL_ATTM:
//		{
//			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
//				GL_TEXTURE_2D, attmUptr->attmID, 0));
//			break;
//		}
//		default:
//		{
//			std::cerr << "Unrecognized Attachment Type!" << std::endl;
//			assert(false);
//		}
//		}
//	}
//};