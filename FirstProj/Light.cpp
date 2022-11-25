#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambient_Intensity = 1.0f;
	diffuse_Intensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambient_Intensity = aIntensity;
	diffuse_Intensity = dIntensity;
}

Light::~Light()
{
}