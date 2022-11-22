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

Material::Material()
{
	specular_Intensity = 0.0f;
	shininess = 0.0f;
}

inline Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specular_Intensity = sIntensity;
	shininess = shine;
}

inline void Material::UseMaterial(GLuint specular_IntensityLoc, GLuint shinenessLoc)
{
	glUniform1f(specular_IntensityLoc, specular_Intensity);
	glUniform1f(shinenessLoc, shininess);
}

Material::~Material()
{
}