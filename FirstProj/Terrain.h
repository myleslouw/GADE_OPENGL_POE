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

	int numStrips;
	int numTriPerStrip;
	glm::mat4 model;
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
	//std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
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
	//std::cout << "Loaded " << indices.size() << " indices" << std::endl;
	IndicesArr = indices.data();


	//Creating a mesh based on the data provided
	Mesh *terrain = new Mesh();
	terrain->createTerrain(vertices,indices);
	//sends Terrain to the back of the list of meshes
	meshList.push_back(terrain);

	numStrips = (Theight - 1) / rez;
	numTriPerStrip = (Twidth / rez) * 2 - 2;

	//	std::cout << "Created lattice of " << numStrips << " strips with " << numTriPerStrip << " triangles each" << std::endl;
		//std::cout << "Created " << numStrips * numTriPerStrip << " triangles total" << std::endl;

	//std::cout << "Raw data: " << vertices.data() << std::endl;
	//std::cout << "Converted Data: " << VertArr << std::endl;
}

inline void Terrain::LoadShaderData()
{
	//loads in the shader data for the Terrain
	Shader *Tshader = new Shader();
	Tshader->CreateFromFiles(vTerrainShader, fTerrainShader);
	shaderList.push_back(Tshader);
}

inline void Terrain::CreateTerrain(glm::mat4 worldProjection, Camera worldCam, int shaderIndex)
{
	//STRIPS FOR DRAWING THE MESH
	const int NumStrips = numStrips;
	const int NumTriPerStrip = numTriPerStrip;

	//USE FOR TERRIAN SHADER
	shaderList[shaderIndex]->useShader(); //glUseProgram
	uniformModel = shaderList[shaderIndex]->getModelLocation();
	uniformProjection = shaderList[shaderIndex]->getProjectionLocation();
	uniformView = shaderList[shaderIndex]->getViewLocation();

	//translation on identty matrix
	model = glm::mat4(1.0f);

	//center terrain to world origin
	model = glm::translate(model, glm::vec3(0, -10.0f, 0));

	//scales for the Terrain
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(worldProjection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(worldCam.calculateViewMatrix()));


	//render the first element which is the Terrain
	meshList[0]->renderTerrainMesh(numStrips, numTriPerStrip);
	//meshList[0]->renderMesh();
}

inline void Terrain::GenerateTerrain(glm::mat4 worldProjection, Camera worldCam)
{
	//RENDER ALL THE ITEMS IN THE LIST
	CreateTerrain(worldProjection, worldCam, 0);
}

Terrain::~Terrain()
{
}
