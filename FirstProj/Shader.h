#pragma once
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "stb_image.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHT = 3;

class Shader
{
public:
	unsigned int texTure;
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePos, uniformSpecular_Int, uniformShininess;
	Shader();


	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	void LoadTexture(const char* fileLocation);
	void UseTexture();
	std::string ReadFile(const char* fileLocation);

#pragma region Location_Methods
	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint getAmbientColourLocation();
	GLuint getDiffuseIntensityLocation();
	GLuint getDirectionLocation();
	GLuint getSpecularIntensityLocation();
	GLuint getShininessLocation();
	GLuint getEyePosition();
#pragma endregion

	void setDirectional_Light(DirectionalLight* dLight);
	void setPoint_Light(PointLight* pLight, unsigned int lightCount);
	void setSpot_Light(SpotLight* sLight, unsigned int lightCount);

	void useShader();
	void clearShader();

	~Shader();

private:

	int pointLightCount;
	int spotLightCount;
#pragma region Light Structs

	//Info for directional light
	struct directional_Light_str
	{
		GLuint uniformColour;
		GLuint uniformAmbient_Int;
		GLuint uniformDiffuse_int;
		GLuint uniformDirection;

	} unifomDirectionalLight;

	GLuint uniformPointLightCount;
	//Info for Point Light 
	struct point_Light_str
	{
		GLuint uniformColour;
		GLuint uniformAmbient_Int;
		GLuint uniformDiffuse_int;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLights[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;
	//Info for SpotLights
	struct spot_Light_str
	{
		GLuint uniformColour;
		GLuint uniformAmbient_Int;
		GLuint uniformDiffuse_Int;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;

	} uniformSpotLights[MAX_SPOT_LIGHT];

#pragma endregion
	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);


};

