#include "PointLight.h"
PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat con, GLfloat lin, GLfloat exp) :
	Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(red, green, blue);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLfloat ambient_IntensityLoc, GLfloat ambient_ColourLoc, GLfloat diffuse_IntensityLoc, GLfloat position_Loc, GLfloat constant_Loc, GLfloat linear_Loc, GLfloat exponent_Loc)
{
	//colour, ambient and diffuse for the light 
	glUniform3f(ambient_ColourLoc, colour.x, colour.y, colour.z);
	glUniform1f(ambient_IntensityLoc, ambient_Intensity);
	glUniform1f(diffuse_IntensityLoc, diffuse_Intensity);

	//positions of point light in the engine
	glUniform3f(position_Loc, position.x, position.y, position.z);
	glUniform1f(constant_Loc, constant);
	glUniform1f(linear_Loc, linear);
	glUniform1f(exponent_Loc, exponent);
}

PointLight::~PointLight()
{
}