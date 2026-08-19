//Autor: Cesar Tadeu Pozzer

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float     brilho;
varying float  x_coord;

void main()
{
   vec2 position = gl_TexCoord[0].st;
   vec3 cor;
   if(x_coord < 0)
      cor = texture2D(texture_0, position).rgb; //nao pega o alpha
   else
      cor = texture2D(texture_1, position).rgb; //nao pega o alpha
   cor*=brilho;
   cor = clamp(cor, 0.0, 1.0); //os parametros 0.0 e 1.0 devem ser float. Nao podem ser inteiros
   
   //gl_FragColor = texture2D(texture, position).rgba;
   gl_FragColor = vec4( cor, 1);
}