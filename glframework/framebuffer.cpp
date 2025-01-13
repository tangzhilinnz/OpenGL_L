#include "framebuffer.h"
#include <vector>
#include "../wrapper/checkError.h"

#include <iostream>

//std::vector<Framebuffer*> Framebuffer::bookmark;
//
//Framebuffer::Framebuffer(int width, int height)
//	: mWidth(width),
//    mHeight(height)
//{}
//
//Framebuffer::~Framebuffer()
//{
//	printf("---- ~Framebuffer ----\n");
//
//	for (auto& fbo : this->mFBOVec)
//	{
//		auto& fboUptr = fbo.second;
//		if (fboUptr->fboID)
//		{
//			GL_CALL(glDeleteFramebuffers(1, &fboUptr->fboID));
//		}
//	}
//
//	for (auto& attm : this->mAttmVec)
//	{
//		auto& attmUptr = attm.second;
//		if (attmUptr->attmID)
//		{
//			if (attmUptr->bufferType == BufferType::RENDER_BUFFER)
//			{
//				GL_CALL(glDeleteRenderbuffers(1, &attmUptr->attmID));
//			}
//			else
//			{
//				GL_CALL(glDeleteTextures(1, &attmUptr->attmID));
//			}
//		}
//	}
//
//	// Set this instance in the static bookmark container to nullptr
//	for (auto& fbo : bookmark)
//	{
//		if (fbo == this)
//		{
//			fbo = nullptr;
//			break;
//		}
//	}
//}
//
//Framebuffer* Framebuffer::createFramebuffer()
//{
//	Framebuffer* framebuffer = new Framebuffer();
//	bookmark.push_back(framebuffer);
//	return framebuffer;
//}
//
//Framebuffer* Framebuffer::createFramebuffer(int width, int height)
//{
//	Framebuffer* framebuffer = new Framebuffer(width, height);
//	bookmark.push_back(framebuffer);
//	return framebuffer;
//}
//
//
//// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//void Framebuffer::genFBO(std::string _fboName)
//{
//	// Check if _fboName is in mFBOVec
//	if (mFBOVec.find(_fboName) != mFBOVec.end())
//	{
//		std::cerr << "Framebuffer: " << _fboName << " already exists"
//			      << std::endl;
//		assert(false);
//		return;
//	}
//
//	// Generate a new framebuffer ID
//	GLuint _fboID = 0;
//	GL_CALL(glGenFramebuffers(1, &_fboID));
//
//	// Create and insert a new FBO object
//	auto fboUptr = std::make_unique<FBO>();
//	fboUptr->fboID = _fboID;
//	fboUptr->fboName = _fboName;
//
//	this->mFBOVec[_fboName] = std::move(fboUptr);
//
//	std::cout << "Generated new framebuffer: " << _fboName << " "
//		      << _fboID << std::endl;
//}
//
//void Framebuffer::genAttm(
//	std::string _attmName, AttachmentType _attmType, BufferType _bufferType,
//	GLenum _internalFormat, GLenum _format, GLenum _type,
//	std::pair<int, int>* _pDims)
//{
//	// Check if attmName is in mAttmVec
//	if (mAttmVec.find(_attmName) != mAttmVec.end())
//	{
//		std::cerr << "Attachment: " << _attmName << " already exists"
//			      << std::endl;
//		assert(false);
//		return;
//	}
//
//	// Create and insert a new Attachment object
//	auto attmUptr = std::make_unique<Attachment>();
//	attmUptr->attmName = _attmName;
//	attmUptr->attmType = _attmType;
//	attmUptr->bufferType = _bufferType;
//	attmUptr->internalFormat = _internalFormat;
//	attmUptr->format = _format;
//	attmUptr->type = _type;
//	attmUptr->width = this->mWidth;
//	attmUptr->height = this->mHeight;
//
//	if (_pDims)
//	{
//		attmUptr->width = (*_pDims).first;
//		attmUptr->height = (*_pDims).second;
//	}
//
//	this->mAttmVec[_attmName] = std::move(attmUptr);
//
//	if (_bufferType == BufferType::RENDER_BUFFER)
//	{
//		this->genRenderbufferAttm(_attmName);
//	}
//	else
//	{
//		this->genTextureAttm(_attmName);
//	}
//
//	std::cout << "Generated new attachment: " << _attmName << std::endl;
//
//}
//
//void Framebuffer::bindFBOAttm(std::string _fboName, std::string _attmName)
//{
//	// Check if _fboName is in mFBOVec
//	if (mFBOVec.find(_fboName) == mFBOVec.end())
//	{
//		std::cerr << "Framebuffer: " << _fboName << " does not exist"
//			      << std::endl;
//		assert(false);
//		return;
//	}
//
//	// Check if attmName is in mAttmVec
//	if (mAttmVec.find(_attmName) == mAttmVec.end())
//	{
//		std::cerr << "Attachment: " << _attmName << " does not exist"
//			      << std::endl;
//		assert(false);
//		return;
//	}
//
//	auto& fboUptr = this->mFBOVec.at(_fboName);
//	auto& attmUptr = this->mAttmVec.at(_attmName);
//
//	// Avoid duplicate attachment binding
//	if (std::find(fboUptr->attms.begin(), fboUptr->attms.end(), _attmName)
//		!= fboUptr->attms.end())
//	{
//		std::cout << "Duplicate attachment binding: " << _attmName << " to "
//			      << _fboName << std::endl;
//		return;
//	}
//
//	fboUptr->attms.push_back(_attmName);
//
//
//	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fboUptr->fboID));
//
//		if (attmUptr->bufferType == BufferType::RENDER_BUFFER)
//		{
//			this->bindRenderbufferAttm(_fboName, _attmName);
//		}
//		else
//		{
//			this->bindTextureAttm(_fboName, _attmName);
//		}
//
//		GL_CALL(glDrawBuffers(fboUptr->drawBuffers.size(), fboUptr->drawBuffers.data()));
//
//		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		{
//			std::cerr << "ERROR::FRAMEBUFFER:: Transparent framebuffer is not complete!"
//				      << std::endl;
//			assert(false);
//		}
//
//	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
//
//	std::cout << "Attachment: " << _attmName << " is bound to "
//		      << "Framebuffer: " << _fboName << std::endl;
//}
//
//void Framebuffer::updateAttm(
//	std::string _attmName, AttachmentType _attmType, BufferType _bufferType,
//	GLenum _internalFormat, GLenum _format, GLenum _type,
//	std::pair<int, int>* _pDims)
//{
//	// Check if _attmName exists in mAttmVec
//	auto it = mAttmVec.find(_attmName);
//	if (it == mAttmVec.end())
//	{
//		std::cerr << "Attachment: " << _attmName << " does not exist" << std::endl;
//		assert(false);
//		return;
//	}
//
//	// Retrieve the existing attachment
//	auto& attmUptr = it->second;
//
//	// Delete the old attachment resource
//	if (attmUptr->attmID)
//	{
//		if (attmUptr->bufferType == BufferType::RENDER_BUFFER) {
//			GL_CALL(glDeleteRenderbuffers(1, &attmUptr->attmID));
//		}
//		else {
//			GL_CALL(glDeleteTextures(1, &attmUptr->attmID));
//		}
//	}
//
//	attmUptr->attmID = 0;
//	attmUptr->attmName = _attmName;
//	attmUptr->attmType = _attmType;
//	attmUptr->bufferType = _bufferType;
//	attmUptr->internalFormat = _internalFormat;
//	attmUptr->format = _format;
//	attmUptr->type = _type;
//	attmUptr->width = this->mWidth;
//	attmUptr->height = this->mHeight;
//
//	if (_pDims)
//	{
//		attmUptr->width = (*_pDims).first;
//		attmUptr->height = (*_pDims).second;
//	}
//
//	// Generate the appropriate OpenGL attachment
//	if (_bufferType == BufferType::RENDER_BUFFER)
//	{
//		this->genRenderbufferAttm(_attmName);
//	}
//	else
//	{
//		this->genTextureAttm(_attmName);
//	}
//
//	std::cout << "Updated attachment: " << _attmName << std::endl;
//}
//
//void Framebuffer::resizeAttm(std::string _attmName, int newWidth, int newHeight)
//{
//	// Check if _attmName exists in mAttmVec
//	auto it = mAttmVec.find(_attmName);
//	if (it == mAttmVec.end())
//	{
//		std::cerr << "Attachment: " << _attmName << " does not exist" << std::endl;
//		assert(false);
//		return;
//	}
//
//	// Retrieve the existing attachment
//	auto& attmUptr = it->second;
//
//	// Delete the old attachment resource
//	if (attmUptr->attmID)
//	{
//		if (attmUptr->bufferType == BufferType::RENDER_BUFFER) {
//			GL_CALL(glDeleteRenderbuffers(1, &attmUptr->attmID));
//		}
//		else {
//			GL_CALL(glDeleteTextures(1, &attmUptr->attmID));
//		}
//	}
//
//	attmUptr->attmID = 0;
//	attmUptr->width = newWidth;
//	attmUptr->height = newHeight;
//
//	// Generate the appropriate OpenGL attachment
//	if (attmUptr->bufferType == BufferType::RENDER_BUFFER)
//	{
//		this->genRenderbufferAttm(_attmName);
//	}
//	else
//	{
//		this->genTextureAttm(_attmName);
//	}
//
//	std::cout << "Resize attachment: " << _attmName << std::endl;
//}
//
//void Framebuffer::resizeFramebuffer(int newWidth, int newHeight)
//{
//	this->mWidth = newWidth;
//	this->mHeight = newHeight;
//
//	for (auto it = this->mAttmVec.begin(); it != this->mAttmVec.end(); ++it)
//	{
//		this->resizeAttm(it->first, this->mWidth, this->mHeight);
//	}
//
//	for (auto it = this->mFBOVec.begin(); it != this->mFBOVec.end(); ++it)
//	{
//		auto& fboUptr = it->second;
//
//		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fboUptr->fboID));
//
//			for (auto& attmName : fboUptr->attms)
//			{
//				auto& attmUptr = this->mAttmVec.at(attmName);
//
//				if (attmUptr->bufferType == BufferType::RENDER_BUFFER)
//				{
//					this->bindRenderbufferAttm(fboUptr->fboName, attmName);
//				}
//				else
//				{
//					this->bindTextureAttm(fboUptr->fboName, attmName);
//				}
//			}
//
//			GL_CALL(glDrawBuffers(fboUptr->drawBuffers.size(), fboUptr->drawBuffers.data()));
//
//			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//			{
//				std::cerr << "ERROR::FRAMEBUFFER:: Transparent framebuffer is not complete!"
//					<< std::endl;
//				assert(false);
//			}
//
//		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
//	}
//}
//
//// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//
//void Framebuffer::destroyAllInstances()
//{
//	for (Framebuffer* instance : bookmark)
//	{
//		if (instance) delete instance;
//	}
//	bookmark.clear();
//}
//
//void Framebuffer::resizeAllInstances(unsigned int newWidth, unsigned int newHeight)
//{
//	for (Framebuffer* framework : Framebuffer::bookmark)
//	{
//		framework->resizeFramebuffer(newWidth, newHeight);
//	}
//}
//


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

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "ERROR::FRAMEBUFFER:: Transparent framebuffer is not complete!"
			      << std::endl;
		assert(false);
	}

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
		std::cerr << "ERROR::FRAMEBUFFER:: Transparent framebuffer is not complete!"
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