#pragma once
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "stb_image.h"


class Shader
{
public:
	unsigned int texTure;
	GLuint shaderID, uniformProjection, uniformModel, uniformView;
	Shader();

	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void LoadTexture(const char* fileLocation);
	std::string ReadFile(const char* fileLocation);

	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	void useShader();
	void clearShader();

	~Shader();

private:
	//GLuint shaderID, uniformProjection, uniformModel, uniformView;

	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

