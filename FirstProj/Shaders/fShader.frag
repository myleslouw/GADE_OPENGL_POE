#version 330
in vec4 vCol;
in vec2 UV;


out vec4 colour;

//texture sampler
uniform sampler2D texture1;

void main()														
{	
	colour = texture(texture1,UV);																					
}