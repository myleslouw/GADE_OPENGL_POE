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
	unsigned int texture;

	void LoadMeshData();
	void LoadShaderData();
	void CreateTerrain(glm::mat4 worldProjection, Camera worldCam, int shaderIndex);
	void GenerateTerrain(glm::mat4 worldProjection, Camera worldCam);

	~Terrain();

private:
	const char* vTerrainShader;
	const char* fTerrainShader;
	const char* textureLoc;
	const char* HeightMap;
	bool USE_TEXTURE;
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	int numStrips;
	int numTriPerStrip;
	glm::mat4 model;
};

Terrain::Terrain()
{
	uniformProjection = 0;
	uniformModel = 0;
	uniformView = 0;
	USE_TEXTURE = true;
	vTerrainShader = "Shaders/vTerrainShader.vert";
	fTerrainShader = "Shaders/fTerrainShader.frag";
	textureLoc = "Textures/Grass_albedo_OG.png";
	HeightMap = "Textures/Grass_height_50x.png";

}

inline void Terrain::LoadMeshData()
{
	stbi_set_flip_vertically_on_load(true);
	int Twidth, Theight, nChannels;
	unsigned char* data = stbi_load(HeightMap, &Twidth, &Theight, &nChannels, 0);
	if (data)
	{
		std::cout << "Loaded heightmap of size " << Theight << " x " << Twidth << std::endl;
	}
	else
	{
		std::cout << "Failed to load Height texture" << std::endl;
	}

#pragma region Vertices Array
	std::vector<float> vertices;
	std::vector<float> UVs;
	float u, v = 0;
	float yScale = 64.0f / 256.0f, yShift = 10.0f; //clamps the y values between 0.0 and 1.0, shifts the points by 16.0f units
	int rez = 1;
	unsigned bytePerPixel = nChannels;
	for (int i = 0; i < Theight; i++)
	{
		for (int j = 0; j < Twidth; j++)
		{
			//retrieve texel for (i,j) tex coord
			unsigned char* pixelOffset = data + (j + Twidth * i) * bytePerPixel;
			unsigned char y = pixelOffset[0];

			// vertex
			vertices.push_back(-Theight / 2.0f + Theight * i / (float)Theight);   // vx
			vertices.push_back((int)y * yScale - yShift);   // vy
			vertices.push_back(-Twidth / 2.0f + Twidth * j / (float)Twidth);   // vz

			//texture Cords
			vertices.push_back((-Theight / 2.0f + Theight * i / (float)Theight) / Twidth); //v.u
			vertices.push_back((-Twidth / 2.0f + Twidth * j / (float)Twidth) / Theight); //v.v
		}
	}
	//std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
	stbi_image_free(data);

	//VertArr = vertices.data();	//Assigns the data from the vector into a pointer GLfloat for the Mesh class
#pragma endregion

#pragma region Indices Array
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
	//IndicesArr = indices.data();
#pragma endregion

	//Creating a mesh based on the data provided
	Mesh *terrain = new Mesh();

	//terrain->CreateMesh(vertices, indices,UVs);
	terrain->CreateMesh(vertices, indices);
	//sends Terrain to the back of the list of meshes
	meshList.push_back(terrain);

	numStrips = (Theight - 1) / rez;
	numTriPerStrip = (Twidth / rez) * 2 - 2;
}

inline void Terrain::LoadShaderData()
{
	//loads in the shader data for the Terrain
	Shader *Tshader = new Shader();
	Tshader->CreateFromFiles(vTerrainShader, fTerrainShader);
	shaderList.push_back(Tshader);
	shaderList[0]->LoadTexture(textureLoc);
}


inline void Terrain::CreateTerrain(glm::mat4 worldProjection, Camera worldCam, int shaderIndex)
{
	//STRIPS FOR DRAWING THE MESH
	const int NumStrips = numStrips;
	const int NumTriPerStrip = numTriPerStrip;

	//is true: use texture, if not, use normal colours
	//USE FOR TEXTURE SHADER
	//shaderList[shaderIndex]->LoadTexture(textureLoc); //gets the texture
	shaderList[shaderIndex]->useShader(); //glUseProgram before the uniforms
	//glUniform1i(glGetUniformLocation(shaderList[shaderIndex]->shaderID, "texture1"), 0);

	//uniforms
	uniformModel = shaderList[shaderIndex]->getModelLocation();
	uniformProjection = shaderList[shaderIndex]->getProjectionLocation();
	uniformView = shaderList[shaderIndex]->getViewLocation();

	//translation on identty matrix
	model = glm::mat4(1.0f);

	//center terrain to world origin
	model = glm::translate(model, glm::vec3(0, -2, 0));

	//scales for the Terrain
	model = glm::scale(model, glm::vec3(1.0f, 0.1f, 1.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(worldProjection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(worldCam.calculateViewMatrix()));

	shaderList[shaderIndex]->UseTexture();
	

	//render the first element which is the Terrain with a specified texture
	meshList[0]->renderMesh(numStrips, numTriPerStrip);
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
