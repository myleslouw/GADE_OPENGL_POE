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
#include "TerrainShader.h"
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

	void LoadImage();
	void LoadMesh();
	void RenderHeightMap(glm::mat4 worldProjection, Camera worldCam);


	~Terrain();

private:
	const char* vTerrainShader;
	const char* fTerrainShader;

	int width, height, nrChannels;

	TerrainShader terrainShader;
	unsigned char* data;
	unsigned int terrainVAO, terrainVBO, terrainIBO;

	int numStrips;
	int numTrisPerStrip;

	GLfloat* VertexArr;
	unsigned int*  IndicesArr;

	GLuint uniformProj = 0, uniformMod = 0, uniformV = 0;
	glm::mat4 Tmodel;

};
