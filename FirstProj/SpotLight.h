#pragma once
#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight();
	SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
			  GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection,
			  GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg);

	void UseLight(GLuint ambient_IntensityLoc, GLuint ambient_ColourLoc,
		GLuint diffuse_IntensityLoc, GLuint position_Loc, GLuint direction_Loc,
		GLuint constant_Loc, GLuint linear_Loc, GLuint exponent_Loc,
		GLuint edge_Loc);

	void setFlash(glm::vec3 pos, glm::vec3 dir);
	~SpotLight();

private:
	glm::vec3 direction;
	GLfloat edge, procEdge;

};
