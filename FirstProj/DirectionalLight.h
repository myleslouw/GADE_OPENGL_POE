#pragma once
#include "Light.h"

//class thatdeals with the actual material of the object
//Determines the specualr value and shininess value of the object
//code references from my lecturer:
//
//
class DirectionalLight: public Light
{
	
public:

	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, 
					 GLfloat xDirection, GLfloat yDirection, GLfloat zDirection);
	void UseLight(GLfloat ambient_IntensityLoc, GLfloat ambient_ColourLoc, GLfloat diffuse_IntensityLoc, GLfloat direction_Loc);
	~DirectionalLight();

private:
	glm::vec3 direction;

};

