#include "Terrain.h"
#include "TerrainShader.h"

int rez = 1;
Mesh* terrainMesh;

Terrain::Terrain()
{
	uniformProj = 0;
	uniformMod = 0;
	uniformV = 0;
	vTerrainShader = "Shaders/vTerrainShader.vert";
	fTerrainShader = "Shaders/fTerrainShader.frag";

	printf("starting shader creation");
	terrainShader = TerrainShader(vTerrainShader, fTerrainShader);

	printf("shader created");

	numStrips = 0;
	numTrisPerStrip = 0;

}

//METHOD 1  - NOT WORKING

void Terrain::GenTerriainData()
{
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Textures/clouds.png", &width, &height, &nrChannels, 0);
	//std::cout << data << std::endl;

	if (data)
	{
		std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}


	std::vector<GLfloat> vertices;
	float yScale = 64.0f / 256.0f, yShift = 16.0f;
	unsigned bytePerPixel = nrChannels;

#pragma region Vertex Gen
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
			unsigned char y = pixelOffset[0];

			// vertex
			vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
			vertices.push_back((int)y * yScale - yShift);   // vy
			vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
		}
	}
	//Sets the data in the array to a GLfloat pointer
	VertexArr = vertices.data();


	/*std::cout << "Vertices:";
	for (size_t i = 0; i < sizeof(vertices) - 1; i++)
	{
		std::cout << vertices[i] << "/";
		std::cout << VertexArr[i];
		std::cout << std::endl;
	}*/
	std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;

	stbi_image_free(data);
#pragma endregion

#pragma region Indices Gen
	//Getting the indices for the array in the image
	std::vector<unsigned int> indices;
	for (unsigned i = 0; i < height - 1; i += rez)
	{
		for (unsigned j = 0; j < width; j += rez)
		{
			for (unsigned k = 0; k < 2; k++)
			{
				indices.push_back(j + width * (i + k * rez));
			}
		}
	}
	IndicesArr = indices.data();
	/*std::cout << "Indices:";
	for (size_t i = 0; i < sizeof(indices) - 1; i++)
	{

		std::cout << indices[i]<< "/";
		std::cout << IndicesArr[i];
		std::cout << std::endl;
	}*/


	std::cout << "Loaded " << indices.size() << " indices" << std::endl;
#pragma endregion


	terrainMesh = new Mesh();
	terrainMesh->createMesh(VertexArr, IndicesArr, sizeof(VertexArr), sizeof(IndicesArr));
	meshList.push_back(terrainMesh);



	//std::cout << "Vertex Data:" << vertices.data() << std::endl;
	//std::cout << "Indices Data:" << indices.data() << std::endl;
	//std::cout << std::endl;
	//std::cout << "VData" << VertexArr << std::endl;
	//std::cout << "IData" << IndicesArr << std::endl;
}

void Terrain::LoadShaderData()
{
	Shader* terrainShader = new Shader();
	terrainShader->CreateFromFiles(vTerrainShader, fTerrainShader);
	shaderList.push_back(terrainShader);

	Tmodel = glm::mat4(1.0f);

	Tmodel = glm::scale(Tmodel, glm::vec3(100.0f, 100.0f, 100.0f));
}

void Terrain::RenderTerrain(glm::mat4 worldProjection, Camera worldCam)
{
	const int numStrips = height - 1;
	const int numTrisPerStrip = width * 2;


	shaderList[0]->useShader();

	uniformMod = shaderList[0]->getModelLocation();//Gets the model projection from the shader
	uniformProj = shaderList[0]->getProjectionLocation(); //Gets the model projection from the shader
	uniformV = shaderList[0]->getViewLocation(); //Gets the view from  shader



	//Matrix  view
	Tmodel = glm::mat4(1.0f);

	Tmodel = glm::scale(Tmodel, glm::vec3(100.0f, 100.0f, 100.0f));

	glUniformMatrix4fv(uniformMod, 1, GL_FALSE, glm::value_ptr(Tmodel));
	glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(worldProjection));
	glUniformMatrix4fv(uniformV, 1, GL_FALSE, glm::value_ptr(worldCam.calculateViewMatrix()));

	//meshList[0]->renderMesh();
	meshList[0]->renderTerrainMesh(numStrips, numTrisPerStrip);
	//terrainMesh->renderMesh();
	/*
	//GLuint VAO, VBO, IBO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);

	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(VertexArr) * sizeof(float), &VertexArr[0], GL_STATIC_DRAW);

	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//glGenBuffers(1, &IBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndicesArr) * sizeof(unsigned), &IndicesArr[0], GL_STATIC_DRAW);

	//glBindVertexArray(VAO);
	//for (unsigned strip = 0; strip < numStrips; strip++)
	//{
	//	glDrawElements(GL_TRIANGLE_STRIP, numTrisPerStrip + 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned)*(numStrips + 2)*strip));
	//}
	//glBindVertexArray(0);

	*/
}


//METHOD 2  - NOT WORKING
void Terrain::LoadImage()
{
	//gets the data from teh heightmap image
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Textures/clouds.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
}

void Terrain::LoadMesh()
{
	//loads the data from the height map into vertices and indices
	std::vector<float> vertices;
	float yScale = 64.0f / 256.0f, yShift = 16.0f;
	int rez = 1;
	unsigned bytePerPixel = nrChannels;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			//loads the image data into vertices
			unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
			unsigned char y = pixelOffset[0];

			// vertex
			vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
			vertices.push_back((int)y * yScale - yShift);   // vy
			vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
		}
	}
	std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
	stbi_image_free(data);

	//image data into indices
	std::vector<unsigned> indices;
	for (unsigned i = 0; i < height - 1; i += rez)
	{
		for (unsigned j = 0; j < width; j += rez)
		{
			for (unsigned k = 0; k < 2; k++)
			{
				indices.push_back(j + width * (i + k * rez));
			}
		}
	}
	std::cout << "Loaded " << indices.size() << " indices" << std::endl;

	numStrips = (height - 1) / rez;
	numTrisPerStrip = (width / rez) * 2 - 2;
	std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
	std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;

	// configuring VAO
	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	// configuring VBO
	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &terrainIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);
}

void Terrain::RenderHeightMap(glm::mat4 worldProjection, Camera worldCam)
{
	//uses the shader
	terrainShader.useShader();

	// translation identity matrix
	glm::mat4 model = glm::mat4(1.0f);
	//center to world origin
	model = glm::translate(model, glm::vec3(0, 0, 0));

	//scales it to the correct dimensions   
	model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));

	terrainShader.setMat4Material("model", model);
	terrainShader.setMat4Material("projection", worldProjection);
	terrainShader.setMat4Material("view", worldCam.calculateViewMatrix());

	// render the terrain
	glBindVertexArray(terrainVAO);

	//drawing elements to the screen
	for (unsigned strip = 0; strip < numStrips; strip++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, numTrisPerStrip + 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned) * (numTrisPerStrip + 2) * strip)); 
	}
}

Terrain::~Terrain()
{

}