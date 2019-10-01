#pragma once
#include <string>
#include <glm/glm.hpp>

// Types of shader supported by the shader class
enum shaderType {
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	GEOMETRY_SHADER,
	PROGRAM
};

class Shader
{
public:
	/**
	* Loads and compiles a shader
	* @param{const char*} Path to the vertex shader
	* @param{const char*} Path to the fragment shader
	*/
	Shader(const char* vertexPath, const char* fragmentPath);

	/**
	* Loads and compiles a shader
	* @param{const char*} Path to the vertex shader
	* @param{const char*} Path to the fragment shader
	* @param{const char*} Path to the geometry shader
	*/
	Shader(const char* vertexPath, const char* fragmentPath, const char* gemotryPath);

	/**
	* Shader destructor
	*/
	~Shader();

	/**
	* Enables the shader to be use
	*/
	void use();
	
	/**
	* Sets a bool uniform
	* @param{std::string &} uniform name
	* @param{bool} value to be set
	*/
	void setBool(const std::string &name, bool value) const;

	/**
	* Sets an int uniform
	* @param{std::string &} uniform name
	* @param{int} value to be set
	*/
	void setInt(const std::string &name, int value) const;
	
	/**
	* Sets an int uniform
	* @param{std::string &} uniform name
	* @param{float} value to be set
	*/
	void setFloat(const std::string &name, float value) const;

	/**
	* Sets an vec2 uniform
	* @param{std::string &} uniform name
	* @param{vec2} value to be set
	*/
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	
	/**
	* Sets an vec2 uniform
	* @param{std::string &} uniform name
	* @param{float} vec2 x value
	* @param{float} vec2 y value
	*/
	void setVec2(const std::string &name, float x, float y) const;

	/**
	* Sets an vec3 uniform
	* @param{std::string &} uniform name
	* @param{vec3} vector value
	*/
	void setVec3(const std::string &name, const glm::vec3 &value) const;

	/**
	* Sets an vec3 uniform
	* @param{std::string &} uniform name
	* @param{float} vec3 x value
	* @param{float} vec3 y value
	* @param{float} vec3 z value
	*/
	void setVec3(const std::string &name, float x, float y, float z) const;
	
	/**
	* Sets an vec4 uniform
	* @param{std::string &} uniform name
	* @param{vec4} vec4 value
	*/
	void setVec4(const std::string &name, const glm::vec4 &value) const;

	/**
	* Sets an vec4 uniform
	* @param{std::string &} uniform name
	* @param{float} vec4 x value
	* @param{float} vec4 y value
	* @param{float} vec4 z value
	* @param{float} vec4 w value
	*/
	void setVec4(const std::string &name, float x, float y, float z, float w);
	
	/**
	* Sets an mat2 uniform
	* @param{std::string &} uniform name
	* @param{mat2} mat2 value
	*/
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	
	/**
	* Sets an mat3 uniform
	* @param{std::string &} uniform name
	* @param{mat3} mat3 value
	*/
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	
	/**
	* Sets an mat4 uniform
	* @param{std::string &} uniform name
	* @param{mat4} mat4 value
	*/
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

	unsigned int ID;
private:
	// Program shader ID in GPU
	

	/**
	* Loads a shader code and compiles it
	* @param{const char*} Path to the shader code
	* @param{shaderType} Type of shader to be compiled
	* @param{unsigned int &} Shader code ID assigned by the GPU, if the code compiles
	* @returns{bool} Compilation status
	*/
	bool compileShaderCode(const char* path, shaderType type, unsigned int &shaderID);

	/**
	* Links individual shader codes into a shader program
	* @param{unsigned int} GPU id of the vertex shader
	* @param{unsigned int} GPU id of the fragment shader
	* @returns{bool} Linking status
	*/
	bool linkProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);

	/**
	* Links individual shader codes into a shader program
	* @param{unsigned int} GPU id of the vertex shader
	* @param{unsigned int} GPU id of the fragment shader
	* @param{unsigned int} GPU id of the geometry shader
	* @returns{bool} Linking status
	*/
	bool linkProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID, unsigned int geometryShaderID);
};

