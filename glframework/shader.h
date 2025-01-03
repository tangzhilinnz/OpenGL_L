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
	~Shader();

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
	void setVector3(const char* name, const glm::vec3 value) const;

	void setInt(const char* name, int value) const;

	void setMatrix4x4(const char* name, glm::mat4 value) const;
	void setMatrix3x3(const char* name, glm::mat3 value) const;
private:
	GLuint compileShader(const char* _shaderPath, GLint _shaderType);

private:
	GLuint mProgram;
};