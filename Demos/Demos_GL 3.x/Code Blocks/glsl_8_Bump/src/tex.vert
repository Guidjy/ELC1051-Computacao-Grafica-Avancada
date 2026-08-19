
//Autor: Cesar Tadeu Pozzer

uniform vec3 lightSourcePos;
//varying vec3 luz;
varying vec3 coord; //coordenada do vertice (pixel) em relacao as coordenadas do objeto

void main()
{
   coord = gl_Vertex.xyz;
   //luz = lightSourcePos;

   gl_Position = ftransform();
   gl_TexCoord[0] = gl_MultiTexCoord0;
}
