#version 330
in vec4 vCol;
in vec2 UV;
in float Use_Tex;

out vec4 colour;

//texture sampler
uniform sampler2D texture1;

void main()														
{	
	if(Use_Tex==1.0f)
	{
		colour = texture(texture1,UV);	
	}

	else
	{
		colour = vCol;
	}																					
}