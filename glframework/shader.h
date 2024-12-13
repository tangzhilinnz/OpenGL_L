#pragma once

#include "core.h"
#include<string>

class Shader
{
public:
	Shader()
		: mProgram(0)
	{}
	Shader(const Shader&) = default;
	Shader& operator=(const Shader&) = default;
	~Shader() = default;

	bool operator==(const Shader& other)
	{
		return this->mProgram == other.mProgram;
	}

	void initShader(const char* _vertexPath, const char* _fragPath, const char* _geoPath = NULL);

	void begin() const;//��ʼʹ�õ�ǰShader
	void end() const;//����ʹ�õ�ǰShader

	GLuint getProgram() const;

	void setFloat(const char* name, float value) const;

	void setVector3(const char* name, float x, float y, float z) const;
	void setVector3(const char* name, const float* values) const;

private:
	GLuint compileShader(const char* _shaderPath, GLint _shaderType);

private:
	GLuint mProgram;
};