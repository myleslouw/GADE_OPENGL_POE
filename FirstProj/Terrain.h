#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <vector>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include "stb_image.h"

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
class Terrain
{
public:
	Terrain();
	std::vector<Mesh*> meshList;		//0 - T
	std::vector<Shader*> shaderList; 

	void LoadMeshData();
	void LoadShaderData();
	void CreateTerrain(glm::mat4 worldProjection, Camera worldCam, int shaderIndex);
	void GenerateTerrain(glm::mat4 worldProjection, Camera worldCam);

	~Terrain();

private:
	const char* vTerrainShader;
	const char* fTerrainShader;
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	GLfloat* VertArr;
	unsigned int* IndicesArr;
};

Terrain::Terrain()
{
	uniformProjection = 0;
	uniformModel = 0;
	uniformView = 0;
	vTerrainShader = "Shaders/vTerrainShader.vert";
	fTerrainShader = "Shaders/fTerrainShader.frag";
}

inline void Terrain::LoadMeshData()
{
	stbi_set_flip_vertically_on_load(true);
	int Twidth, Theight, nChannels;
	unsigned char* data = stbi_load("Textures/Heightmap2.png", &Twidth, &Theight, &nChannels, 0);
	if (data)
	{
		std::cout << "Loaded heightmap of size " << Theight << " x " << Twidth << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	std::vector<float> vertices;
	float yScale = 64.0f / 256.0f, yShift = 16.0f; //clamps the y values between 0.0 and 1.0, shifts the points by 16.0f units
	int rez = 1;
	unsigned bytePerPixel = nChannels;
	for (int i = 0; i < Theight; i++)
	{
		for (int j = 0; j < Twidth; j++)
		{
			unsigned char* pixelOffset = data + (j + Twidth * i) * bytePerPixel;
			unsigned char y = pixelOffset[0];

			// vertex
			vertices.push_back(-Theight / 2.0f + Theight * i / (float)Theight);   // vx
			vertices.push_back((int)y * yScale - yShift);   // vy
			vertices.push_back(-Twidth / 2.0f + Twidth * j / (float)Twidth);   // vz
		}
	}
	std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
	stbi_image_free(data);

	VertArr = vertices.data();	//Assigns the data from the vector into a pointer GLfloat for the Mesh class

	std::vector<unsigned> indices;
	for (unsigned i = 0; i < Theight - 1; i += rez)
	{
		for (unsigned j = 0; j < Twidth; j += rez)
		{
			for (unsigned k = 0; k < 2; k++)
			{
				indices.push_back(j + Twidth * (i + k * rez));
			}
		}
	}
	std::cout << "Loaded " << indices.size() << " indices" << std::endl;
	IndicesArr = indices.data();


	//Creating a mesh based on the data provided
	Mesh *terrain = new Mesh();
	terrain->createMesh(VertArr, IndicesArr, sizeof(VertArr), sizeof(IndicesArr));
	//sends Terrain to the back of the list of meshes
	meshList.push_back(terrain);
}

inline void Terrain::LoadShaderData()
{
	//loads in the shader data for the Terrain
	Shader *Tshader = new Shader();
	Tshader->CreateFromFiles(vTerrainShader,fTerrainShader);
	shaderList.push_back(Tshader);
}

Terrain::~Terrain()
{
}
