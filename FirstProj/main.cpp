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
#include "FPSCounter.h"	//fps counter
#include "SkyBox.h"		//skybox
#include "Material.h" //Materials for the objects in scene
#include "DirectionalLight.h" //Generates the mainlight in the scene
#include "PointLight.h" //Generates the PointLight for the scene
#include "SpotLight.h"	//Generate the SpotLight for the scene

const float degreeToRadians = 3.14159265 / 100.0f;

Window mainWindow;

//Terrain terrain;
ChessBoard chessboard;
Terrain heightmap;
FPSCounter fpsCounter;
SkyBox skybox;

Material nShinyMat;
Material nDullMat;

//Lighting objects
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



	heightmap = Terrain();

	//create chessboard obj
	chessboard = ChessBoard();

	heightmap.LoadMeshData();	//loads mesh data and creates the mesh object
	heightmap.LoadShaderData();	//loads the shader data from our shader file

	chessboard.LoadMeshes();		//createobjects
	chessboard.LoadShaders();		//createShaders

	fpsCounter = FPSCounter();

	//ENGINE MATERIALS
	nShinyMat = Material(1.0f, 32);
	nDullMat = Material(0.3f, 4);


	//CAMERA HERE
	//sets the global instance to the current scene camera
	globalCamera = Camera(glm::vec3(0.0f, 10.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -45.0f, 2.0f, 0.5f);

#pragma region ENGINE LIGHTING
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,	//RBG
								0.0f, 0.0f,			//Ambient and Diffuse Intensity
								0.0f, 0.0f, -1.0f);	//xyz Direction

	//POINT LIGHT
	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(0.0f, 1.0f, 1.0f,	//RBG
								0.0f, 1.0f,			//Ambient and Diffuse Intensity
								3.0f, 0.0f, 0.0f,	//xyz Position
								0.3f,0.2f, 0.1f);	//con, lin, exp

	pointLightCount++; //comment this line of code to see the spot light in the scene
	

	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,	//RBG
								0.0f, 1.0f,			//Ambient and Diffuse Intensity
								-4.0f, 0.0f, 0.0f,	//xyz Position
								0.3f, 0.1f, 0.1f);	//con, lin, exp
	pointLightCount++;

	unsigned int spotLightCount = 0;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,		//RBG
							  0.0f, 2.0f,			//Ambient and Diffuse Intensity
							  0.0f, 0.0f, 0.0f,		//xyz Position
							  0.0f,-1.0f, 0.0f,		//xyz Direction
							  1.0f, 0.0f, 0.0f,		//con, lin, exp
							  30.0f);				//edges
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,		//RGB
							  0.0f, 1.0f,			//Ambient and Diffuse Intesity
							  0.0f,-1.5f, 0.0f,		//xyz Position
							 -100.0f,-1.0f, 0.0f,	//xyz Direction
							  1.0f, 0.0f, 0.0f,		//con, lin, exp
							  45.0f);				//edges
//	spotLightCount++;
	
#pragma endregion

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);


	//loads textures into vector
	std::vector<std::string> skyboxFaces;

	//SKYBOX IMAGES (ONLY THIS WORKS, ALL OTHERS HAVE A SIDEWAYES TOP AND BOTTOM
	//LAKE
#pragma region SKY BOX CODE
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = SkyBox(skyboxFaces);
#pragma endregion

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
		//skybox
		skybox.DrawSkybox(globalCamera.calculateViewMatrix(), projection);

		//fps counter

		fpsCounter.ShowFPS(mainWindow.getMainWindow(), glfwGetTime());

		//generate heightmap/terrain
	//	heightmap.GenerateTerrain(projection, globalCamera);

		//generates the chessboard
		chessboard.GenerateChessBoard(projection, globalCamera, mainLight, pointLights, spotLights, 3, 3);

		chessboard.AnimateChessPieces(projection, globalCamera, deltaTime, mainLight, pointLights, spotLights, 3, 3);
		//------------------------------------------------------------------

		glUseProgram(0);	//unassigning the shader

		mainWindow.SwapBuffers();
		glfwPollEvents();
	}
	// glfw: Terminate, clearing all prev allocated GLFW resources
	glfwTerminate();
	return 0;
}