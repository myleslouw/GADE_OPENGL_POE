#include "ChessBoard.h"

ChessBoard::ChessBoard()
{
	uniformProjection = 0;
	uniformModel = 0;
	uniformView = 0;
	vColourShader = "Shaders/vColourShader.vert";
	vWhiteShader = "Shaders/vWhiteShader.vert";
	vBorderShader = "Shaders/vBorderShader.vert";
	fShader = "Shaders/fShader.frag";
	
	lowestHeight = -20;
	highestHeight = 70;

	srand(time(NULL));		//srand seed taken from https://stackoverflow.com/questions/9459035/why-does-rand-yield-the-same-sequence-of-numbers-on-every-run

	//fill array with random heights for the blocks
	for (size_t i = 0; i < sizeof(rndmHeights[0]); i++)
	{
		for (size_t j = 0; j < sizeof(rndmHeights[1]); j++)
		{
			//heights are randomed every restart
			rndmHeights[i][j] = GetRandomHeight();;
		}
	}
}

void ChessBoard::LoadMeshes()
{

	//ALL CHESS ITEMS ARE A CUBE MESH THAT IS STRETCHED TO DIFFERENT SIZES
	unsigned int cubeIndices[] = {
		//Top
	   2, 6, 7,
	   2, 3, 7,

	   //Bottom
	   0, 4, 5,
	   0, 1, 5,

	   //Left
	   0, 2, 6,
	   0, 4, 6,

	   //Right
	   1, 3, 7,
	   1, 5, 7,

	   //Front
	   0, 2, 3,
	   0, 1, 3,

	   //Back
	   4, 6, 7,
	   4, 5, 7
	};

	//equates to 1 unit 
	GLfloat CubeVertices[] = {
	   -0.5, -0.5,  0.5, //0
		 0.5, -0.5,  0.5, //1
		-0.5,  0.5,  0.5, //2
		 0.5,  0.5,  0.5, //3
		-0.5, -0.5, -0.5, //4
		 0.5, -0.5, -0.5, //5
		-0.5,  0.5, -0.5, //6
		 0.5,  0.5, -0.5  //7
	};

	//create obj
	Mesh *cube = new Mesh();
	//create obj mesh
	cube->CreateMesh(CubeVertices, cubeIndices, sizeof(CubeVertices), sizeof(cubeIndices));
	//adds it to list of meshes
	meshList.push_back(cube);
}

void ChessBoard::LoadShaders()
{
	//white
	Shader *shader1 = new Shader();							//creates obj
	shader1->CreateFromFiles(vWhiteShader, fShader);		//creates shader
	shaderList.push_back(shader1);							//adds to list

	//black
	Shader *shader2 = new Shader();
	shader2->CreateFromFiles(vColourShader, fShader);
	shaderList.push_back(shader2);

	//border/grey
	Shader *shader3 = new Shader();
	shader3->CreateFromFiles(vBorderShader, fShader);
	shaderList.push_back(shader3);
}

void ChessBoard::CreateBorderBlock(glm::mat4 worldProjection, Camera worldCam, int shaderIndex)
{
	//THIS IS ONLY USED FOR BORDER
	//gets the shader from the param
	shaderList[shaderIndex]->useShader();		//glUseProgram
	uniformModel = shaderList[shaderIndex]->getModelLocation();
	uniformProjection = shaderList[shaderIndex]->getProjectionLocation();
	uniformView = shaderList[shaderIndex]->getViewLocation();

	//translation on identity matrix
	model = glm::mat4(1.0f);

	//center to world origin
	model = glm::translate(model, glm::vec3(0, 0, 0));

	//scales it to the correct dimensions   
	model = glm::scale(model, glm::vec3(9.0f, 0.5f, 9.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(worldProjection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(worldCam.calculateViewMatrix()));

	//renders the first element which is the border
	meshList[0]->renderMesh();
}

void ChessBoard::CreateCellBlock(glm::mat4 worldProjection, Camera worldCam, int shaderIndex, glm::vec3 pos, glm::vec3 scale)
{
	////USED FOR CREATING ALL THE BLOCKS ON THE CHESSBOARD
	//uses the shader given via params
	shaderList[shaderIndex]->useShader();		//glUseProgram
	uniformModel = shaderList[shaderIndex]->getModelLocation();
	uniformProjection = shaderList[shaderIndex]->getProjectionLocation();
	uniformView = shaderList[shaderIndex]->getViewLocation();

	//translation on identity matrix
	model = glm::mat4(1.0f);

	//sets the scale to param
	model = glm::scale(model, scale);

	//translates the block to given position (to make chessboard shape)
	model = glm::translate(model, pos);

	//view matrix stuff
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(worldProjection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(worldCam.calculateViewMatrix()));

	//render the obj
	meshList[0]->renderMesh();
}

void ChessBoard::GenerateChessBoard(glm::mat4 worldProjection, Camera worldCam )
{
	//THIS RENDERS ALL THE ITEMS FOR THE CHESS BOARD

	//1st
	//border
	CreateBorderBlock(worldProjection, worldCam, 2);

	//2nd
	//8 by 8 grid of blocks
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			//initial offset so the centre is at (0,0,0)
			float x = i - 3.5f;
			float z = j - 3.5f;

			//shader selector
			//alternating black and white blocks taken from Alexei Sholik's comment		https://stackoverflow.com/questions/16347627/create-a-black-and-white-chessboard-in-a-two-dimensional-array
			int shaderNum;
			shaderNum = ((i + j) % 2) ? 0 : 1;	//if there is a remainder after adding I and J, it will be black

			//get height from array of random heights based on the index (the heights are randomed on start so each run is differnet)
			float height = rndmHeights[i][j];

			//renders the block with the correct info										//1 by 1 by 1 block 
			CreateCellBlock(worldProjection, worldCam, shaderNum, glm::vec3(x, height, z), glm::vec3(1, 1, 1));
		}
	}

}

float ChessBoard::GetRandomHeight()
{
	//https://stackoverflow.com/questions/12580820/random-number-between-1-to-10-using-c
	//used to get number between range ^

	float rndm;
	int range = (highestHeight - lowestHeight) + 1;
	rndm = lowestHeight + rand() % range;
	return rndm/100; //to get a float between 0 and 1
}

ChessBoard::~ChessBoard()
{
}
