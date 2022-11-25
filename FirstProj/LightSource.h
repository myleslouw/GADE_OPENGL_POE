#pragma once
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"

class LightSource
{
public:

	LightSource();
	void LoadShaderData();
	void RenderMesh();

	~LightSource();

private:
	const char* vShader;
	const char* fshader;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	Mesh* LightCube;
};
