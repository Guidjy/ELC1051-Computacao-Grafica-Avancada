// Fragment Shader
//
// Este Fragment Shader amostra a textura com id 'colorTextureSampler' e aplicada 
// esta amostra ao fragmento.
// Nenhum cálculo de iluminação é feito.
//
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 400

uniform sampler2D colorTextureSampler;

in vec2 teTexCoord;

void main()
{
	gl_FragColor = texture2D(colorTextureSampler, teTexCoord);
}