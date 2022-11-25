#include "LightSource.h"

LightSource::LightSource()
{
	uniformProjection = 0;
	uniformModel = 0;
	uniformView = 0;
	vShader = "";
	fshader = "";

	unsigned int LightIndices[] = {
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

	GLfloat LightVertices[] = {
		//Positions			
		-0.5, -0.5,  0.5,	//0 bottom left
		 0.5, -0.5,  0.5,	//1 bottom right
		-0.5,  0.5,  0.5,	//2 top left
		 0.5,  0.5,  0.5,	//3 top right
		-0.5, -0.5, -0.5,	//4 bottom left (back)
		 0.5, -0.5, -0.5,	//5 bottom right (back)
		-0.5,  0.5, -0.5,	//6 top left (top back)
		 0.5,  0.5, -0.5,	

	};
	LightCube->CreateLightMesh(LightVertices,LightIndices,sizeof(LightVertices),sizeof(LightIndices));
	LoadShaderData();
}
 void LightSource::LoadShaderData()
{
	 Shader* LShader = new Shader();
	 LShader->CreateFromFiles(vShader,fshader);
}

 void LightSource::RenderMesh()
 {
	 LightCube->renderMesh();
 }

 LightSource::~LightSource()
 {
 }
