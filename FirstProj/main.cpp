#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//created files
#include "Window.h"
#include "CamManager.h"		//used for storing global instance of the camera 
#include "ChessBoard.h"	//Generate the chessboard 
#include "Terrain.h"	//Generates the heightmap data
#include "DirectionalLight.h" //Generates the mainlight in the scene
#include "PointLight.h" //Generates the PointLight for the scene
#include "SpotLight.h"	//Generate the SpotLight for the scene

const float degreeToRadians = 3.14159265 / 100.0f;

Window mainWindow;

//Terrain terrain;
ChessBoard chessboard;
Terrain heightmap;
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHT];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

int main()
{
	//window
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	//CAMERA HERE
	//sets the global instance to the current scene camera
	globalCamera = Camera(glm::vec3(0.0f, 10.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -45.0f, 2.0f, 0.5f);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

#pragma region ENGINE LIGHTING
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, -1.0f);

	//POINT LIGHT
	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.3f,
		0.2f, 0.1f);
	pointLightCount++; //comment this line of code to see the spot light in the scene

	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, -4.0f,
		2.0f, 0.0f, 0.3f,
		0.1f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 20.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		-1.5f, 0.0f, -100.0f,
		-1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 20.0f);
	spotLightCount++;
#pragma endregion

	//TERRAIN STUFF HERE
	heightmap = Terrain();
	heightmap.LoadMeshData();	//loads mesh data and creates the mesh object
	heightmap.LoadShaderData();	//loads the shader data from our shader file

		//CHESS BOARD STUFF HERE
	chessboard = ChessBoard();
	chessboard.LoadMeshes();		//createobjects
	chessboard.LoadShaders();		//createShaders
	CalcAverageNormals(chessboard.meshIndices,sizeof(chessboard.meshIndices),chessboard.meshVertices,sizeof(chessboard.meshVertices),8,5);

	//loop until window closed
	while (!mainWindow.getShouldClose())
	{
		//to run at a constant FPS regardless of PC
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Get and Handle user input events
			//checks if any evetns happened
		globalCamera.keyControl(mainWindow.getKeys(), deltaTime);
		globalCamera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//setting window Colour
		//			R		G	  B		Alpha
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		//-------------------------------------------------------------
		//generate heightmap/terrain
		heightmap.GenerateTerrain(projection, globalCamera);

		//generates the chessboard
		chessboard.GenerateChessBoard(projection, globalCamera);

#pragma region ChessBoard Lighting
		//adding lighting to the specfic shader of an object in the scene
		//for the white blocks
		chessboard.shaderList[0]->setDirectional_Light(&mainLight);
		chessboard.shaderList[0]->setPoint_Light(pointLights, pointLightCount);
		chessboard.shaderList[0]->setSpot_Light(spotLights, spotLightCount);

		chessboard.shaderList[1]->setDirectional_Light(&mainLight);
		chessboard.shaderList[1]->setPoint_Light(pointLights, pointLightCount);
		chessboard.shaderList[1]->setSpot_Light(spotLights, spotLightCount);

		chessboard.shaderList[2]->setDirectional_Light(&mainLight);
		chessboard.shaderList[2]->setPoint_Light(pointLights, pointLightCount);
		chessboard.shaderList[2]->setSpot_Light(spotLights, spotLightCount);
#pragma endregion
		//chessboard.AnimateChessPieces(projection, globalCamera, deltaTime);
		//------------------------------------------------------------------

		glUseProgram(0);	//unassigning the shader

		mainWindow.SwapBuffers();
		glfwPollEvents();
	}
	// glfw: Terminate, clearing all prev allocated GLFW resources
	glfwTerminate();
	return 0;
}

void CalcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices,
	unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}