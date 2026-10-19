// Geometry Shader
//
// A primitiva de entrada está disponível no geometry shader através da variável gl_in. Note que ela
// é na verdade um array. Isto se da pelo fato de que o geometry shader pode receber triângulos, linhas
// e pontos, primitavas que possuem mais de um vértice.
// A geração do quadrilátero é feita da seguinte forma:
//		1. Calcule os atributos do vértice e associe os valores para as variáveis de saída (gl_Position e gColor).
//		2. Emita o vértice (envie ele para o próximo estágio do pipeline) através da fuñção EmitVertex().
// Assim que todos os vértices foram emitidos, é necessário chamar a função EndPrimitive() para finalizar a primitiva.
//
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br


#version 400

// In the geometry shader, we start by defining the kind of primitive that this geometry shader
// expects to receive. The first layout statement indicates that this geometry shader will receive
// point primitives.
layout( points ) in;

// The next layout statement indicates the kind of primitives produced by this geometry shader,
// and the maximum number of vertices that will be output.
// In this case, we want to produce a single quad for each point received, so we indicate that the
// output will be a triangle strip with a maximum of four vertices.
layout( triangle_strip, max_vertices = 4 ) out;

uniform mat4 ProjectionMatrix;

in vec3 vColor[];

out vec3 gColor;

void main()
{
	float size = 0.15;
	
	gl_Position = ProjectionMatrix * (vec4(-size, -size, 0.0, 0.0) + gl_in[0].gl_Position);
	gColor = vColor[0];
	EmitVertex();
	
	gl_Position = ProjectionMatrix * (vec4(size, -size, 0.0, 0.0) + gl_in[0].gl_Position);
	gColor = vColor[0];
	EmitVertex();
	
	gl_Position = ProjectionMatrix * (vec4(-size, size, 0.0, 0.0) + gl_in[0].gl_Position);
	gColor = vColor[0];
	EmitVertex();
	
	gl_Position = ProjectionMatrix * (vec4(size, size, 0.0, 0.0) + gl_in[0].gl_Position);
	gColor = vColor[0];
	EmitVertex();

	EndPrimitive();
}