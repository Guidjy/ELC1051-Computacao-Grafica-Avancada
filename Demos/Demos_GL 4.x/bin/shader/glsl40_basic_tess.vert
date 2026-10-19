// Vertex Shader
//
// Como as tranformções serão aplicadas somente no estágio Tessellation Evaluation Shader, este
// vertex shader apenas passa os vértices de entrada adiante para o Tessellation Control Shader.
//
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 400

layout(location = 0) in vec3 VertexPosition;

void main()
{
	gl_Position = vec4(VertexPosition, 1.0);
}
