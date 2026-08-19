//Autor: Cesar Tadeu Pozzer

varying float cor1, cor2;

void main()
{
   //gl_FragColor = vec4(0.4, 0.4, 0.8,1.0);
   gl_FragColor = vec4(cor1, cor2, 0, 1.0);
   //gl_FragColor = gl_Color;
}