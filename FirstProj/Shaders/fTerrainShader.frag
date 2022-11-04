#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float Height;

//texture sampler
uniform sampler2D texture1;

void main()
{
   // float h = (Height + 16)/32.0f;	// shift and scale the height in to a grayscale value
    FragColor = texture(texture1,TexCoord);
}
