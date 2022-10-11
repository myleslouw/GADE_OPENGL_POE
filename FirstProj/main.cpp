#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
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

const float degreeToRadians = 3.14159265 / 100.0f;

Window mainWindow;

//Terrain terrain;
ChessBoard chessboard;
Terrain heightmap;

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

	heightmap = Terrain();

	//create chessboard obj
	chessboard = ChessBoard();

	heightmap.LoadMeshData();	//loads mesh data and creates the mesh object
	heightmap.LoadShaderData();	//loads the shader data from our shader file

	chessboard.LoadMeshes();		//createobjects
	chessboard.LoadShaders();		//createShaders



	//loop until window closed
	while (!mainWindow.getShouldClose())
	{
		//to run at a constant FPS regardless of PC
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Get and Handle user input events
		glfwPollEvents();		//checks if any evetns happened
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

		//------------------------------------------------------------------

		glUseProgram(0);	//unassigning the shader

		mainWindow.SwapBuffers();
	}
}