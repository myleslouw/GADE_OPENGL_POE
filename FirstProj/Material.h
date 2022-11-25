#pragma once
#include <GL/glew.h>

//class thatdeals with the actual material of the object
//Determines the specualr value and shininess value of the object
//code references from my lecturer:
//
//
class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint specular_IntensityLoc, GLuint shinenessLoc);
	~Material();

private:
	GLfloat specular_Intensity;
	GLfloat shininess;
};
