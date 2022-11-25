#pragma once
#include <GL\glew.h>

#include <stdio.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* fileLoc);

	bool LoadTexture(); //returns true if texture was loaded
	bool LoadTextureA(); //same as above, but for textures with an alpha channel

	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	char* fileLocation;
};