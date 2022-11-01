#version 330
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 VertexUV;
vec3 cubeColour = vec3(1.0f, 1.0f, 1.0f);

out vec4 vCol;
out vec2 UV;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	//vCol = vec4(clamp(cubeColour, 0.0f, 1.0f), 1.0);
	UV = vec2(VertexUV.x,VertexUV.y);
}