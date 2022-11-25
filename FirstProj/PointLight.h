#pragma once
#include "Light.h"

class PointLight:public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLfloat ambient_IntensityLoc, GLfloat ambient_ColourLoc, 
				  GLfloat diffuse_IntensityLoc, GLfloat position_Loc,
				  GLfloat constant_Loc, GLfloat linear_Loc, GLfloat exponent_Loc);

	~PointLight();

protected:
	glm::vec3 position;
	GLfloat constant, linear, exponent;
};
