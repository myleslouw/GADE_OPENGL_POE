#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat xDirection, GLfloat yDirection, GLfloat zDirection,
	GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg) :
	PointLight(red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
{
	direction = glm::normalize(glm::vec3(xDirection, yDirection, zDirection));
	edge = edg;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambient_IntensityLoc, GLuint ambient_ColourLoc,
	GLuint diffuse_IntensityLoc, GLuint position_Loc,
	GLuint direction_Loc, GLuint constant_Loc,
	GLuint linear_Loc, GLuint exponent_Loc, GLuint edge_Loc)
{
	//ambient colour, intensity, and diffuse
	glUniform3f(ambient_ColourLoc, colour.x, colour.y, colour.z);
	glUniform1f(ambient_IntensityLoc, ambient_Intensity);
	glUniform1f(diffuse_IntensityLoc, diffuse_Intensity);

	//position location for the light source
	glUniform3f(position_Loc, position.x, position.y, position.z);
	glUniform1f(constant_Loc, constant);
	glUniform1f(linear_Loc, linear);
	glUniform1f(exponent_Loc, exponent);

	//direction that the light is pointing to 
	glUniform3f(direction_Loc, direction.x, direction.y, direction.z);
	glUniform1f(edge_Loc, procEdge);
}

inline void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight ::~SpotLight()
{
}