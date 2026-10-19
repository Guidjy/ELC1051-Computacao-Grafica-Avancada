// Vertex Shader
//
// Este vertex shader é extremamente simples. Ele apenas converte a posição do vértice utilizando
// a matriz ModelView. Também é passado adiante uma cor para cada vértice. A cor é determinada pela
// posição do vértice.
//
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 400

layout (location = 0) in vec3 VertexPosition;

uniform mat4 ModelViewMatrix;
out vec3 vColor;

void main()
{	
	vColor = VertexPosition;
	gl_Position = ModelViewMatrix * vec4(VertexPosition,1.0);
}