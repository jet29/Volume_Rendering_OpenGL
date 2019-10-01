#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
	unsigned vertexID, fragmentID;

	if (!compileShaderCode(vertexPath, shaderType::VERTEX_SHADER, vertexID))
		return;

	if (!compileShaderCode(fragmentPath, shaderType::FRAGMENT_SHADER, fragmentID))
	{
		glDeleteShader(vertexID);
		return;
	}

	linkProgram(vertexID, fragmentID);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
{
	unsigned vertexID, fragmentID, geometryID;

	if (!compileShaderCode(vertexPath, shaderType::VERTEX_SHADER, vertexID))
		return;

	if (!compileShaderCode(fragmentPath, shaderType::FRAGMENT_SHADER, fragmentID))
	{
		glDeleteShader(vertexID);
		return;
	}

	if (!compileShaderCode(geometryPath, shaderType::GEOMETRY_SHADER, geometryID))
	{
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
		return;
	}

	linkProgram(vertexID, fragmentID, geometryID);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	glDeleteShader(geometryID);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

bool Shader::compileShaderCode(const char *path, shaderType type, unsigned int &shaderID)
{
	std::string shaderCode;
	std::ifstream shaderFile;

	// Set exceptions for ifstream object
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open the file
		shaderFile.open(path);
		std::stringstream shaderStream;
		// Reads the buffer content into streams
		shaderStream << shaderFile.rdbuf();
		// Close the file handler
		shaderFile.close();
		// Convert the stream into a string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER Error reading file: " << path << std::endl;
		return false;
	}

	const char *code = shaderCode.c_str();
	std::string stringType;
	// Creates the shader object in the GPU
	switch (type)
	{
	case VERTEX_SHADER:
		shaderID = glCreateShader(GL_VERTEX_SHADER);
		stringType = "VERTEX";
		break;
	case FRAGMENT_SHADER:
		shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		stringType = "FRAGMENT";
		break;
	case GEOMETRY_SHADER:
		shaderID = glCreateShader(GL_GEOMETRY_SHADER);
		stringType = "GEOMETRY";
		break;
	}
	// Loads the shader code to the GPU
	glShaderSource(shaderID, 1, &code, NULL);
	// Compiles the shader
	glCompileShader(shaderID);

	int succes;
	char log[1024];
	// Get compilation status
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &succes);
	// Compilation error
	if (!succes)
	{
		// Gets the error message
		glGetShaderInfoLog(shaderID, 1024, NULL, log);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << stringType << "\n"
				  << "path: "
				  << path
				  << "\n " << log << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}

	return true;
}

bool Shader::linkProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID)
{
	// Creates GPU shader program
	ID = glCreateProgram();
	// Attach the vertex shader for linking
	glAttachShader(ID, vertexShaderID);
	// Attach the fragment shader for linking
	glAttachShader(ID, fragmentShaderID);
	// Link the shaders
	glLinkProgram(ID);

	int succes;
	char log[1024];
	// Get compilation status
	glGetShaderiv(ID, GL_LINK_STATUS, &succes);
	// Compilation error
	if (!succes)
	{
		// Gets the error message
		glGetShaderInfoLog(ID, 1024, NULL, log);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR\n"
				  << log << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}

	// Delete all the shaders because they are no longer neccesary
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return true;
}

bool Shader::linkProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID, unsigned int geometryShaderID)
{
	// Creates GPU shader program
	ID = glCreateProgram();
	// Attach the vertex shader for linking
	glAttachShader(ID, vertexShaderID);
	// Attach the fragment shader for linking
	glAttachShader(ID, fragmentShaderID);
	// Attach the geometry shader for linking
	glAttachShader(ID, geometryShaderID);
	// Link the shaders
	glLinkProgram(ID);

	int succes;
	char log[1024];
	// Get compilation status
	glGetShaderiv(ID, GL_LINK_STATUS, &succes);
	// Compilation error
	if (!succes)
	{
		// Gets the error message
		glGetShaderInfoLog(ID, 1024, NULL, log);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR\n"
				  << log << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}

	// Delete all the shaders because they are no longer neccesary
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteShader(geometryShaderID);

	return true;
}
