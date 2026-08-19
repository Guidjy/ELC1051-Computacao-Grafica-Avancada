//Autor: Cesar Tadeu Pozzer

//parametros passsados pela aplicacao. 
attribute vec4  cor;
attribute float scalar;

//parametros passados para o fragment shader. 
//Nao se pode passar variaveis attribute para o fragment shader. Por isso deve-se criar uma variavel varying 
//para armazenar este valor. 
varying vec4 color;
varying float temperature;

void main()
{	
   color = cor;
   temperature = scalar;
 
   gl_Position = ftransform();
}
