//Autor: Cesar Tadeu Pozzer

uniform sampler2D texture;
varying vec3 luz, normal, coord;
uniform vec3 lightSourcePos;

void main()
{
   float intensity;
   //vec3 luz = vec3(0.0, 0.0, -9.5);
   vec3 normal = texture2D(texture, gl_TexCoord[0].st).rgb; //cor entre [0,1]
   vec3 vetDirLuz;

   vetDirLuz = lightSourcePos - coord;
   vetDirLuz = normalize(vetDirLuz);

   //transforma as componentes da textura de [0,1] em rgb para [-1, 1], que sao as coodenadas do vetor normal em xyz
   normal *= 2.0;
   normal -= 1.0;
   //normal = normalize(normal);

   intensity = max(dot(normal,vetDirLuz),0.0);
   gl_FragColor = vec4(intensity,intensity,intensity,0);

   //para teste
   //if(luz.z < -1.0 )
   //   gl_FragColor = vec4(0,1,0,0);
   //gl_FragColor = vec4(0,1,0,0);

}
