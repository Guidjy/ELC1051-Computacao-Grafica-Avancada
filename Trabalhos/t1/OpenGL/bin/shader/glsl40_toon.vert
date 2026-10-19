// simple toon vertex shader
// www.lighthouse3d.com

#version 400

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexColor;
layout (location=2) in vec3 VertexNormal;

out vec3 Color;
out vec3 Normal;

uniform mat4 MVP;
uniform mat3 NormalMatrix;

void main()
{
    Color = VertexColor;
	Normal = NormalMatrix * VertexNormal;

	gl_Position = MVP * vec4(VertexPosition, 1.0);
}
