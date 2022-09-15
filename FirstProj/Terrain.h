#pragma once

//STB IMAGES
#include "stb_image.h"

//GLEW
#include<GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"


class Terrain
{
public:

	//list of obj meshes and list of shaders available
	std::vector<Mesh*> meshList;		//0 - cube
	std::vector<Shader*> shaderList;	//0 - white, 1 - black,  2 - grey for border

	Terrain();

	void GenTerriainData();
	void LoadShaderData();

	void RenderTerrain(glm::mat4 worldProjection, Camera worldCam);

	~Terrain();

private:
	const char* vBorderShader;
	const char* fShader;

	int width, height, nChannels;
	unsigned char* data;
	GLfloat* VertexArr;
	unsigned int*  IndicesArr;

	GLuint uniformProj = 0, uniformMod = 0, uniformV = 0;
	glm::mat4 Tmodel;

};
