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

	texture1 = "Textures/BrickSoviet_OG.png";
	texture2 = "Textures/BrickSloppy_OG.png";
	texture3 = "Textures/Pavement_OG.png";

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

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

	chessAnimation = ChessAnimation();
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
		//Positions				UVs		Nx	  Ny   Nz
		-0.5, -0.5,  0.5,	0.0f,0.0f,	0.0f,0.0f,0.0f,		//0 bottom left
		 0.5, -0.5,  0.5,	1.0f,0.0f,	0.0f,0.0f,0.0f,		//1 bottom right
		-0.5,  0.5,  0.5,	0.0f,1.0f,	0.0f,0.0f,0.0f,		//2 top left
		 0.5,  0.5,  0.5,	1.0f,1.0f,	0.0f,0.0f,0.0f,		//3 top right
		-0.5, -0.5, -0.5,	-1.0f,0.0f,	0.0f,0.0f,0.0f,		//4 bottom left (back)
		 0.5, -0.5, -0.5,	1.0f,-1.0f,	0.0f,0.0f,0.0f,		//5 bottom right (back)
		-0.5,  0.5, -0.5,	0.0,2.0f,	0.0f,0.0f,0.0f,		//6 top left (top back)
		 0.5,  0.5, -0.5,	1.0f,2.0f,	0.0f,0.0f,0.0f		//7 top right ( top back)
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

	//Calls the method to load in the textures for the specfic object
	shaderList[0]->LoadTexture(texture1);
	shaderList[1]->LoadTexture(texture2);
	shaderList[2]->LoadTexture(texture3);
}

//Creates the base that the chess board will be on 
void ChessBoard::CreateBorderBlock(glm::mat4 worldProjection, Camera worldCam, int shaderIndex)
{
	//THIS IS ONLY USED FOR BORDER
	//gets the shader from the param
	//shaderList[shaderIndex]->LoadTexture(texture3);
	shaderList[shaderIndex]->useShader();		//glUseProgram
	//glUniform1i(glGetUniformLocation(shaderList[shaderIndex]->shaderID, "texture"), 0);
	//uniforms
	uniformModel = shaderList[shaderIndex]->getModelLocation();
	uniformProjection = shaderList[shaderIndex]->getProjectionLocation();
	uniformView = shaderList[shaderIndex]->getViewLocation();
	uniformEyePos = shaderList[shaderIndex]->getEyePosition();
	uniformSpecular_Int = shaderList[shaderIndex]->getSpecularIntensityLocation();

	//translation on identity matrix
	model = glm::mat4(1.0f);

	//center to world origin
	model = glm::translate(model, glm::vec3(0, 0, 0));

	//scales it to the correct dimensions   
	model = glm::scale(model, glm::vec3(9.0f, 0.5f, 9.0f));

	//uniforms for GL
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(worldProjection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(worldCam.calculateViewMatrix()));

	//Applying shiny specular
	glUniform3f(uniformEyePos, worldCam.getCameraPosition().x, worldCam.getCameraPosition().y, worldCam.getCameraPosition().z);
	shinyMaterial.UseMaterial(uniformSpecular_Int, uniformShininess);
	//Textures
	shaderList[shaderIndex]->UseTexture();

	//renders the first element which is the border
	meshList[0]->renderMesh();
}

void ChessBoard::CreateCellBlock(glm::mat4 worldProjection, Camera worldCam, int shaderIndex, glm::vec3 pos, glm::vec3 scale)
{
	////USED FOR CREATING ALL THE BLOCKS ON THE CHESSBOARD
	//uses the shader given via params

	
	shaderList[shaderIndex]->useShader();		//glUseProgram
	//glUniform1i(glGetUniformLocation(shaderList[shaderIndex]->shaderID, "texture"), 0);
	//uniform stuff
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

	//Textures
	shaderList[shaderIndex]->UseTexture();
	//render the obj
	meshList[0]->renderMesh();
}

void ChessBoard::GenerateChessBoard(glm::mat4 worldProjection, Camera worldCam)
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
			float height = 0; //rndmHeights[i][j];

			//renders the block with the correct info										//1 by 1 by 1 block 
			CreateCellBlock(worldProjection, worldCam, shaderNum, glm::vec3(x, height, z), glm::vec3(1, 1, 1));
		}
	}
}

void ChessBoard::AnimateChessPieces(glm::mat4 worldProjection, Camera worldCam, GLfloat deltaTime)
{
	//draws the pieces
	//the position param is taken from the vec3 array and those are changed to move the pieces
	for (size_t i = 0; i < 16; i++)
	{
		CreateCellBlock(worldProjection, worldCam, 2, glm::vec3(chessAnimation.chessPieces[i].x, chessAnimation.chessPieces[i].y, chessAnimation.chessPieces[i].z), glm::vec3(0.5, 1, 0.5));
	}

	//animation cycle
	chessAnimation.MovePiece(deltaTime);
}

float ChessBoard::GetRandomHeight()
{
	//https://stackoverflow.com/questions/12580820/random-number-between-1-to-10-using-c
	//used to get number between range ^

	float rndm;
	int range = (highestHeight - lowestHeight) + 1;
	rndm = lowestHeight + rand() % range;
	return rndm / 100; //to get a float between 0 and 1
}

ChessBoard::~ChessBoard()
{
}
