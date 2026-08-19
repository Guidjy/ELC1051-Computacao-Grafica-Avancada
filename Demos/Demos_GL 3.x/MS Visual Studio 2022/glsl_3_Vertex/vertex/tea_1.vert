
//Autor: Cesar Tadeu Pozzer

//#version 150

uniform float time;
varying float cor1, cor2;

void main()
{
      vec4 v = vec4(gl_Vertex);
      v.z = v.z + sin(v.z*10.0+time)/10.0;
      v.x = v.x + sin(v.x*10.0+time)/10.0;

      cor1 = (sin(v.z*10.0)+2.0)/2.0;
      cor2 = (sin(v.x*5.0)+2.0)/2.0;

      // the following three lines provide the same result
      //gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
      //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
      //gl_Position = ftransform();

      gl_Position = gl_ModelViewProjectionMatrix * v;
}
