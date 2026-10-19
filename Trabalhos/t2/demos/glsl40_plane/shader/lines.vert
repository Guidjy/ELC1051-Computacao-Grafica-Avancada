/*
	Transforms each vertex by the MVP matrix (projection * view * model)
	and passes its color on to the fragment shader.
*/
#version 400

layout (location = 0) in vec3 VertexPosition;  // VBO attribute 0
layout (location = 1) in vec3 VertexColor;     // VBO attribute 1

out vec3 Color;

uniform mat4 MVP;

void main()
{
	Color = VertexColor;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}
