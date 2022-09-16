//#include <glad/glad.h>
#include "TerrainShader.h"


//USED FOR METHOD 2

TerrainShader::TerrainShader()
{

}

TerrainShader::TerrainShader(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();


	//Compiling the shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// set up fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// set up shader Program
	terrainShaderID = glCreateProgram();
	glAttachShader(terrainShaderID, vertex);
	glAttachShader(terrainShaderID, fragment);
	glLinkProgram(terrainShaderID);
	checkCompileErrors(terrainShaderID, "PROGRAM");
	// delete shaders after they have been linked
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

void TerrainShader::useShader()
{
	//used to activate the shader
	glUseProgram(terrainShaderID);
}

void TerrainShader::setVec4Value(const std::string &name, const glm::vec4 &value)
{
	glUniform4fv(glGetUniformLocation(terrainShaderID, name.c_str()), 1, &value[0]);
}

void TerrainShader::setVec4Position(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(terrainShaderID, name.c_str()), x, y, z, w);
}

void TerrainShader::setMat4Material(const std::string &name, const glm::mat4 &mat)
{
	glUniformMatrix4fv(glGetUniformLocation(terrainShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

//error checkin
void TerrainShader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

