//Autor: Cesar Tadeu Pozzer
//2012

varying vec3 luzPos, normal, coord;

void main()
{
   float intensity;
   vec3 vetDirLuz;

   //o vetor direcao deve ser normalizado apos a subtracao.
   vetDirLuz = normalize( luzPos - coord );

   intensity = max(dot(normal,vetDirLuz),0.0);

   gl_FragColor = vec4(intensity,intensity,intensity,0);

}
