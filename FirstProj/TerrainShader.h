#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//USED FOR METHOD 2

class TerrainShader
{
public:
	unsigned int terrainShaderID;


	TerrainShader();
	TerrainShader(const char* vertexPath, const char* fragmentPath);

	void useShader();

	//used for setting uniform variables
	void setVec4Value(const std::string &name, const glm::vec4 &value);

	void setVec4Position(const std::string &name, float x, float y, float z, float w);

	void setMat4Material(const std::string &name, const glm::mat4 &mat);

	void checkCompileErrors(GLuint shader, std::string type);
};