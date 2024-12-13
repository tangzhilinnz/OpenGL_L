#include"shader.h"
#include"../wrapper/checkError.h"
#include <assert.h>

void Shader::initShader(const char* _vertexPath, const char* _fragPath, const char* _geoPath)
{
    GLuint vertexID = compileShader(_vertexPath, GL_VERTEX_SHADER);
    GLuint fragID = compileShader(_fragPath, GL_FRAGMENT_SHADER);
    GLuint geoID = compileShader(_geoPath, GL_GEOMETRY_SHADER);

    assert(vertexID != -1 && fragID != -1);

    char infoLog[512];
    int  successFlag = 0;

    this->mProgram = glCreateProgram();

    glAttachShader(this->mProgram, vertexID);
    glAttachShader(this->mProgram, fragID);

    if (geoID != -1)
    {
        glAttachShader(this->mProgram, geoID);
    }

    glLinkProgram(this->mProgram);

    glGetProgramiv(this->mProgram, GL_LINK_STATUS, &successFlag);
    if (!successFlag)
    {
        glGetProgramInfoLog(this->mProgram, 512, NULL, infoLog);
        printf("Shader Program Linking Error: %s\n", infoLog);
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragID);
    
    if (geoID != -1)
    {
        glDeleteShader(geoID);
    }
}

GLuint Shader::compileShader(const char* _shaderPath, GLint _shaderType)
{
    if (_shaderPath == "" || _shaderPath == NULL)
    {
        return -1;
    }

    // File pointers
    FILE* shaderFile = nullptr;

    // Buffers for file contents
    char* shaderCode = nullptr;

    errno_t err;

    // Open vertex shader file
    err = fopen_s(&shaderFile, _shaderPath, "rb");
    if (err != 0)
    {
        perror("Error opening vertex shader file");
        return -1;
    }

    // Get file sizes
    fseek(shaderFile, 0, SEEK_END);
    size_t shaderSize = ftell(shaderFile);
    rewind(shaderFile);

    // Allocate memory for file contents
    shaderCode = new char[shaderSize + 1];

    if (!shaderCode)
    {
        perror("Error: Failed to allocate memory for shader code.\n");
        fclose(shaderFile);
        delete[] shaderCode;
        return -1;
    }

    // Read file contents
    fread(shaderCode, 1, shaderSize, shaderFile);

    // Null-terminate strings
    shaderCode[shaderSize] = '\0';

    // Close files
    fclose(shaderFile);

    // Compile shader
    GLuint shaderID;
    char infoLog[512];
    int successFlag;

    shaderID = glCreateShader(_shaderType);
    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &successFlag);
    if (!successFlag)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        printf("Vertex Shader Compilation Error: %s\n", infoLog);
        return -1;
    }

    delete[] shaderCode;

    return shaderID;
}

void Shader::begin() const
{
	GL_CALL(glUseProgram(this->mProgram));
}

void Shader::end() const
{
	GL_CALL(glUseProgram(0));
}

GLuint Shader::getProgram() const
{
    return this->mProgram;
}

void Shader::setFloat(const char* name, float value) const
{
    GLint location;
    GL_CALL(location=glGetUniformLocation(mProgram, name));
    assert(location != -1);
    GL_CALL(glUniform1f(location, value));
}

void Shader::setVector3(const char* name, float x, float y, float z) const
{
    GLint location;
    GL_CALL(location=glGetUniformLocation(mProgram, name));
    assert(location != -1);
    GL_CALL(glUniform3f(location, x, y, z));
}

//重载 overload
void Shader::setVector3(const char* name, const float* values) const
{
    GLint location;
    GL_CALL(location=glGetUniformLocation(mProgram, name));
    assert(location != -1);
    //第二个参数：你当前要更新的uniform变量如果是数组，数组里面包括多少个向量vec3
    GL_CALL(glUniform3fv(location, 1, values));
}