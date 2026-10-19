// Fragment Shader
//
// Simples Fragment Shader que seta a cor de todos os fragmentos para vermelho.
// Nenhum cálculo de iluminação é feito.
//
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 400

void main()
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}