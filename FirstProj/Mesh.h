#pragma once

#include <GL/glew.h>
#include <vector>
class Mesh
{
public: 
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numVertices, unsigned int numIndices);
	void CreateSkyboxMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void CreateMesh(std::vector<float> Verts, std::vector<unsigned>Inds);
	void renderMesh();
	void renderMesh(const int numStrips, const int numTrisPerStrip);

	void clearMesh();


	~Mesh();

private: 
	GLuint VAO, VBO, IBO, TBO;
	int indexCount;

};

