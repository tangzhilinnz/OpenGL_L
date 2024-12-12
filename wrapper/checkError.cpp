#include "checkError.h"
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <assert.h>


void checkError(const char* func, const char* file, int line)
{
	GLenum errorCode = glGetError();

	if (errorCode != GL_NO_ERROR)
	{
		const char* error = nullptr;
		switch (errorCode)
		{
			case GL_INVALID_ENUM:      error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:     error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
			case GL_OUT_OF_MEMORY:     error = "OUT_OF_MEMORY"; break;
			default:                   error = "UNKNOWN_ERROR"; break;
		}

		std::cerr
			<< "[OpenGL Error] " << error
			<< " in function " << func
			<< " at " << file << ":" << line << std::endl;

		assert(false); // Halt execution in debug builds
	}
}