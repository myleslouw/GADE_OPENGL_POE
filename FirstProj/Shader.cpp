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

	compileLightingShader(vertexCode, fragmentCode);
}

void Shader::CreateSkyboxFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	compileSkyboxShader(vertexCode, fragmentCode);
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

	//mipmaps
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//load image, create texture and gen mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); //tell stbi_image.h to flip loaded textures on the y-axis.
	unsigned char* TexData = stbi_load(fileLocation, &width, &height, &nrChannels, 0);
	if (TexData)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TexData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load Image texture" << std::endl;
	}
	stbi_image_free(TexData);
}

void Shader::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texTure);
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

void Shader::compileSkyboxShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	compileProgram();
}
//Method that compiles that shader,
//Also used for creating the directional, point, and spot light
void Shader::compileLightingShader(const char* vertexCode, const char* fragmentCode)
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

#pragma region Uniforms
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	unifomDirectionalLight.uniformAmbient_Int = glGetUniformLocation(shaderID, "DiretionalLight.base.ambient_Intensity");	//update start
	unifomDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "DiretionalLight.base.colour");
	unifomDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "DiretionalLight.direction");
	unifomDirectionalLight.uniformDiffuse_Int = glGetUniformLocation(shaderID, "DirectionalLight.base.diffuse_Intensity");	//update end
	uniformSpecular_Int = glGetUniformLocation(shaderID, "Material.specular_Intensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePos = glGetUniformLocation(shaderID, "eyePosition");
#pragma endregion

	//code for generating the Point Light
	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	//PointLight Generation
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' }; //this is a null terminator for end of string


		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambient_Intensity", (int)i);
		uniformPointLights[i].uniformAmbient_Int = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", (int)i);
		uniformPointLights[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuse_Intensity", (int)i);
		uniformPointLights[i].uniformDiffuse_Int = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", (int)i);
		uniformPointLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", (int)i);
		uniformPointLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", (int)i);
		uniformPointLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", (int)i);
		uniformPointLights[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	//SpotLight Generation
	for (size_t i = 0; i < MAX_SPOT_LIGHT; i++)
	{
		char locBuff[100] = { '\0' }; //this is a null terminator for the string

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambient_Intensity", (int)i);
		uniformSpotLights[i].uniformAmbient_Int = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", (int)i);
		uniformSpotLights[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuse_Intensity", (int)i);
		uniformSpotLights[i].uniformDiffuse_Int = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", (int)i);
		uniformSpotLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", (int)i);
		uniformSpotLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", (int)i);
		uniformSpotLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", (int)i);
		uniformSpotLights[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", (int)i);
		uniformSpotLights[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", (int)i);
		uniformSpotLights[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}
}

void Shader::compileProgram() {

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformView = glGetUniformLocation(shaderID, "view");
	
	//uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	
}

#pragma region GET_METHODS
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
GLuint Shader::getAmbientIntensityLocation()
{
	return unifomDirectionalLight.uniformAmbient_Int;
}
GLuint Shader::getAmbientColourLocation()
{
	return unifomDirectionalLight.uniformColour;
}
GLuint Shader::getDiffuseIntensityLocation()
{
	return unifomDirectionalLight.uniformDiffuse_Int;
}
GLuint Shader::getDirectionLocation()
{
	return unifomDirectionalLight.uniformDirection;
}
GLuint Shader::getSpecularIntensityLocation()
{
	return uniformSpecular_Int;
}
GLuint Shader::getShininessLocation()
{
	return uniformShininess;
}
GLuint Shader::getEyePosition()
{
	return uniformEyePos;
}
#pragma endregion


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

void Shader::Validate()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

void Shader::setDirectional_Light(DirectionalLight* dlight)
{
	dlight->UseLight(unifomDirectionalLight.uniformAmbient_Int,
		unifomDirectionalLight.uniformColour, unifomDirectionalLight.uniformDiffuse_Int,
		unifomDirectionalLight.uniformDirection);
}

void Shader::setPoint_Lights(PointLight* plight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);
	for (size_t i = 0; i < lightCount; i++)
	{
		plight[i].UseLight(uniformPointLights[i].uniformAmbient_Int, uniformPointLights[i].uniformColour,
			uniformPointLights[i].uniformDiffuse_Int, uniformPointLights[i].uniformPosition,
			uniformPointLights[i].uniformConstant, uniformPointLights[i].uniformLinear,
			uniformPointLights[i].uniformExponent);
	}
}

void Shader::setSpot_Lights(SpotLight* slight, unsigned int lightCount)
{
	if (spotLightCount > MAX_SPOT_LIGHT) lightCount = MAX_SPOT_LIGHT;
	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		slight[i].UseLight(uniformSpotLights[i].uniformAmbient_Int, uniformSpotLights[i].uniformColour,
			uniformSpotLights[i].uniformDiffuse_Int, uniformSpotLights[i].uniformPosition, uniformSpotLights[i].uniformDirection,
			uniformSpotLights[i].uniformConstant, uniformSpotLights[i].uniformLinear,
			uniformSpotLights[i].uniformExponent, uniformSpotLights[i].uniformEdge);
	}
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
