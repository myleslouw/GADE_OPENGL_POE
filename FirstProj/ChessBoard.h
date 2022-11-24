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

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "ChessAnimation.h"
#include "Material.h"

class ChessBoard 
{
public:
	//list of obj meshes and list of shaders available
	std::vector<Mesh*> meshList;		//0 - cube
	std::vector<Shader*> shaderList;	//0 - white, 1 - black,  2 - grey for border

	GLfloat* meshVertices;
	unsigned int* meshIndices;
	Material shinyMaterial;
	Material dullMaterial;

	ChessAnimation chessAnimation;

	ChessBoard();

	void LoadMeshes();

	void LoadShaders();



	void CreateBorderBlock(glm::mat4 worldProjection, Camera worldCam, int shaderIndex);

	void CreateCellBlock(glm::mat4 worldProjection, Camera worldCam, int shaderIndex, glm::vec3 pos, glm::vec3 scale);

	void GenerateChessBoard(glm::mat4 worldProjection, Camera worldCam);

	void AnimateChessPieces(glm::mat4 worldProjection, Camera worldCam, GLfloat deltaTime);


	~ChessBoard();

private:
	const char* vColourShader;
	const char* vWhiteShader;
	const char* vBorderShader;
	const char* fShader;

	const char* texture1;
	const char* texture2;
	const char* texture3;



	float GetRandomHeight();

	GLuint	uniformProjection = 0, uniformModel = 0, uniformView = 0, 
			uniformEyePos = 0, uniformSpecular_Int = 0, uniformShininess = 0;

	void CalculateAVGNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices,
		unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset);
	glm::mat4 model;

	//array for a 8 by 8 grid of random heights
	float lowestHeight, highestHeight;
	float rndmHeights[8][8];
};

