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

	void begin() const;//开始使用当前Shader
	void end() const;//结束使用当前Shader

	GLuint getProgram() const;

	void setFloat(const char* name, float value) const;

	void setVector3(const char* name, float x, float y, float z) const;
	void setVector3(const char* name, const float* values) const;

	void setInt(const char* name, int value);

	void setMatrix4x4(const char* name, glm::mat4 value);
private:
	GLuint compileShader(const char* _shaderPath, GLint _shaderType);

private:
	GLuint mProgram;
};