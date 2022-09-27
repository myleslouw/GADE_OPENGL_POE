#version 330 core

out vec4 FragColor;

in vec2 UV;
in float Height;

uniform sampler2D ourTexture;

void main()
{
    float h = (Height + 16)/32.0f;	// shift and scale the height in to a grayscale value
    FragColor = vec4(h, h, h, 1.0);
    //FragColor = texture(ourTexture,TexCoord);
}
