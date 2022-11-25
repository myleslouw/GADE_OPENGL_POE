#include "Material.h"

Material::Material()
{
	specular_Intensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specular_Intensity = sIntensity;
	shininess = shine;
}

void Material::UseMaterial(GLuint specular_IntensityLoc, GLuint shinenessLoc)
{
	glUniform1f(specular_IntensityLoc, specular_Intensity);
	glUniform1f(shinenessLoc, shininess);
}

Material::~Material()
{
}