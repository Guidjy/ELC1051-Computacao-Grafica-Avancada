/*
	Recebe a posicao do vertice e sua respectiva cor.
	Transforma o vertice e passa a cor para o FS
*/
#version 400 // versao minima da GLSL utilizada. Pode ser usada para invocar funcionalidades de versoes especificas da linguagem.

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexColor;

out vec3 Color;

uniform mat4 MVP;
uniform bool isShaderGammaCorrectionEnabled;

void main()
{
	if (isShaderGammaCorrectionEnabled)
	{
		Color = pow(VertexColor, vec3(1.0/2.2));
	}
	else
	{
		Color = VertexColor;
	}
	
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}
