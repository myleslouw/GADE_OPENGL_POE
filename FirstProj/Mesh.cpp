#include "Mesh.h"
#include <iostream>

//done in class
Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::createMesh(GLfloat* vertices, unsigned int* indices, unsigned int numVertices, unsigned int numIndices)
{
	indexCount = numIndices;


	//generates a vertex array with the VBO
	glGenVertexArrays(1, &VAO);
	//binds it
	glBindVertexArray(VAO);

#pragma region Creating IBO
	//creates a buffer for the IBO
	glGenBuffers(1, &IBO);
	//binds the IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);
#pragma endregion


#pragma region creatingVBO
	//create a buffer with 1 VBO
	glGenBuffers(1, &VBO);

	//binding VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//connecting the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW);
#pragma endregion
	//(LocationOfAttribute,
   //XYZ which are 3 values,
   //TypeOfValues, 
   //NormaliseValues,
   //Stride(take a vertex value and skip n amount)
   // Offset(where the data starts))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//                     \\
 	//                      \\   
	//enable Attribute 0     \/
	glEnableVertexAttribArray(0);


	//unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glBindVertexArray(0);
}

//Custom VAO buffers for the Terrain 
void Mesh::createTerrain(std::vector<float> Verts, std::vector<unsigned> Inds)
{

	//setting up VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Buffer Data for the Vertices
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Verts.size() * sizeof(float), &Verts[0], GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//BufferData for the Indices
	glGenBuffers(1,&IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Inds.size() *sizeof(unsigned),&Inds[0], GL_STATIC_DRAW);

}

void Mesh::renderMesh()
{
	//binding VAO
	glBindVertexArray(VAO);
	//bind the IBO to buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//drawing to screen
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//unbinding after drawing
	glBindVertexArray(0);
}

void Mesh::renderTerrainMesh(const int numStrips, const int numTrisPerStrip)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


	for (unsigned strip = 0; strip < numStrips; strip++)
	{
		glDrawElements(GL_TRIANGLE_STRIP,	//primitive type
			numTrisPerStrip + 2,			//number of indices to render
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned) * (numTrisPerStrip + 2) *strip)); //offset to starting index
	}
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//unbinding the Array after drawing
	//glBindVertexArray(0);
	//std::cout << "nS: " << numStrips << "nTri: " << numTrisPerStrip;
}


void Mesh::clearMesh()
{
	//clears the mesh on destruction
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh() {
	clearMesh();
}