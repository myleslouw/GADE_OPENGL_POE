#pragma once

#include <vector>
#include <string>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

class SkyBox
{
public:
	SkyBox();
	//stores the locations of the faces of the skybox
	SkyBox(std::vector<std::string> faceLocations);

	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	~SkyBox();

private:
	Mesh* skyboxMesh;
	Shader* skyboxShader;

	GLuint textureId;
	GLuint uniformProjection, uniformView;
};

