#define STB_IMAGE_IMPLEMENTATION
#include "Shader.h"
//done in class
Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode);
}
//Method used to call texture from File
void Shader::LoadTexture(const char* fileLocation)
{

	//Texture 
	glGenTextures(1, &texTure);
	glBindTexture(GL_TEXTURE_2D, texTure);

	//texture wrapping para
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//set texture filtering para
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//load image, create texture and gen mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); //tell stbi_image.h to flip loaded textures on the y-axis.
	unsigned char* TexData = stbi_load(fileLocation, &width, &height, &nrChannels, 0);
	if (TexData)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TexData);
	}
	else
	{
		std::cout << "Failed to load Image texture" << std::endl;
	}
	stbi_image_free(TexData);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode)
{
	//creating the prog
	shaderID = glCreateProgram();

	// make sure the shader is created correctly.
	if (!shaderID)
	{
		printf("Failed to create shader\n");
		return;
	}

	// adding shader to the program
	// pass in the prog, string vShader
	// indicate the type of shader
	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);


	// adding shader to the program
	// pass in the prog, string fShader
	// indicate the type of shader
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

#pragma region ErrorChecking
	//  create the roles on the gpu
	   //linking the prog and making sure
	   //everything is working
	   //check if its linked properly and validate the settings
	   //for the openGL

	//================================================================

	// getting error codes from the creation
	// of the shaders
	GLint result = 0; // result of the two functions
	GLchar eLog[1024] = { 0 }; // logging the error

	//checking if the program is linked correctly
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // get the info. Check if the prog is linked

	//check if the result is false.
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	//validate the program/
	// checking if openGL was setup correctly for the shader
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
#pragma endregion

	//uniformModel = glGetUniformLocation(shader, "xMove");
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");

}

GLuint Shader::getProjectionLocation()
{
	return uniformProjection;
}
GLuint Shader::getModelLocation()
{
	return uniformModel;
}

GLuint Shader::getViewLocation()
{
	return uniformView;
}

void Shader::useShader()
{
	if (!shaderID)
	{
		printf("Failed to use shader\n");
		return;
	}
	glUseProgram(shaderID);

}

void Shader::clearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;

}



void Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType); // create the individual shader

	const GLchar* theCode[1]; //array with a pointer to the first element of the array
	theCode[0] = shaderCode;

	GLint codeLength[1]; // length of the code
	codeLength[0] = (GLint)strlen(shaderCode); // get the length of the code

	//Passing the created shader code into the shader source
	glShaderSource(theShader, 1, theCode, codeLength); //modifies the source for the shader
	glCompileShader(theShader); // compile the shader code that is in memory
#pragma region ErrorCheck


	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}
#pragma endregion

	//attaching the shader to the program
	glAttachShader(theProgram, theShader);


}

Shader::~Shader()
{
	clearShader();

}
