
//Autor: Cesar Tadeu Pozzer

varying vec4 color;
varying float temperature;
uniform float time;
uniform int   mode;


void main()
{
  
   if( mode == 1 )
   {
      if(color.r > color.g && color.r > color.b)
	     gl_FragColor = vec4(sin(color.r*50.0+time), 0, 0, 1);
      if(color.g > color.r && color.g > color.b)
         gl_FragColor = vec4(0, tan(color.g*40.0+time), 0, 1);
      if(color.b > color.r && color.b > color.g)
         gl_FragColor = vec4(0, 0, sin(color.b*50.0-time), 1);
   }	   
   else if (mode == 2 )
   {
      if(color.r > color.g && color.r > color.b)
         gl_FragColor = vec4(1, 0, 0, 1);
      if(color.g > color.r && color.g > color.b)
         gl_FragColor = vec4(0, color.g, 0, 1);
      if(color.b > color.r && color.b > color.g)
         gl_FragColor = vec4(0, 0, color.b, 1);
   }
   else if (mode == 3)
   {
      gl_FragColor = color;
   }
   else 
   {
      float cor = temperature;
      if( temperature > 0.4)
          cor += 0.2;
      gl_FragColor = vec4(cor, cor, cor, 1);
   }
      
}