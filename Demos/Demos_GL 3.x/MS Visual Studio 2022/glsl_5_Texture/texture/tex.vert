
//Autor: Cesar Tadeu Pozzer

//uniform sampler2D texture;
varying float  x_coord;
varying float  y_coord;

void main()
{	
   x_coord = gl_Vertex.x;
   y_coord = gl_Vertex.y;
   gl_Position = ftransform();
   
   //gl_TexCoord[0]     --> built-in varying variable
   //gl_MultiTexCoord0  --> built-in attribute variable
   gl_TexCoord[0] = gl_MultiTexCoord0;  
}
