#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

//class that
//Determines the specualr value and shininess value of the object
//code references from my lecturer:
//
//
class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambient_Intensity;
	GLfloat diffuse_Intensity;
};
