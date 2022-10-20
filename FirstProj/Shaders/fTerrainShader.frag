#version 330 core
out vec4 FragColor;

in vec2 UV;
in float Height;
in float Use_Tex;

//texture sampler
uniform sampler2D texture1;

void main()
{
    if(Use_Tex ==1.0f)
    {
        float h = (Height + 16)/32.0f;	// shift and scale the height in to a grayscale value
        //FragColor = vec4(h, h, h, 1.0);
        FragColor = texture(texture1,UV);
    }
    else
    {
         float h = (Height + 16)/32.0f;	// shift and scale the height in to a grayscale value
        FragColor = vec4(h, h, h, 1.0);
        //FragColor = texture(texture1,UV);
    }
}
