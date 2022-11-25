#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::SkyBox(std::vector<std::string> faceLocations)
{
	// Shader Setup
	skyboxShader = new Shader();
	skyboxShader->CreateSkyboxFromFiles("Shaders/vSkybox.vert", "Shaders/fSkybox.frag");

	//setting up uniforms
	uniformProjection = skyboxShader->getProjectionLocation();
	uniformView = skyboxShader->getViewLocation();


	// Texture Setup
	glGenTextures(1, &textureId);		//using only 1 texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);	//binding texture (using a cube map)

	int width, height, bitDepth;
	//otherwise image is upside down
	stbi_set_flip_vertically_on_load(false);

	for (size_t i = 0; i < 6; i++)
	{
		//getting the data from the texture 
		unsigned char *texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
		if (!texData)
		{
			printf("Failed to find: %s\n", faceLocations[i].c_str());
			return;
		}
		//storing the data as unsigned bytes
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		//free the image after attaching 
		stbi_image_free(texData);
	}
	//cube map texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Mesh Setup
	unsigned int skyboxIndices[] = {
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};

	float skyboxVertices[] = {
		//vertices				//tex coords	//colours
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	skyboxMesh = new Mesh();
	skyboxMesh->CreateSkyboxMesh(skyboxVertices, skyboxIndices, 64, 36);
}

void SkyBox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	//IMPORTANT!!! no depth test because the skybox draws around the camera and everything else shows through
	glDepthMask(GL_FALSE);
	//uses the shader
	skyboxShader->useShader();

	glScalef(1.0f, 1.0f, 1.0f);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	skyboxShader->Validate();
	//renders the skybox
	skyboxMesh->renderMesh();

	//then enables the depth mask so its only off while drawing skybox
	glDepthMask(GL_TRUE);
}

SkyBox::~SkyBox()
{
}
