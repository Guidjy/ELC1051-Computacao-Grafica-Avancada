//Autor: Cesar Tadeu Pozzer

void main()
{
   //teste de inicializacao de diagonal de matrizes
   //***********************************************************************
   mat2 m = mat2(1); //somente a diagonal principal recebe 1. O resto recebe 0
   /*
   if( m[0][1] == 1 )
      gl_FragColor = vec4( 1,0,0, 1);
   else
      gl_FragColor = vec4( 0,0,1, 1);
   /* */


   //teste de acesso a matrizes
   //***********************************************************************

   //1 1
   //2 2
   m[0][0] = 1.0;  //comandos para montar a matriz acima
   m[1][0] = 1.0;
   m[0][1] = 2.0;
   m[1][1] = 2.0;
   vec2 v = m[0];  //a indexacao eh por coluna.
   if(v.y == 2.0)
      gl_FragColor = vec4( 1,0,0, 1);
   else
      gl_FragColor = vec4( 0,0,1, 1);
   /* */

   //teste de operacoes component-wise
   //***********************************************************************
   vec3 vet = vec3(0.0, 1.0, 2.0), res;
   res = cos(vet);
   if(res.x == res.y)
      gl_FragColor = vec4( 1,0,0, 1);
   else
      gl_FragColor = vec4( 0,0,1, 1);
   //gl_FragColor = vec4( res.x, res.y, res.z, 1);
   /* */

   //teste de relacionais component-wise
   //***********************************************************************
   vec2  v1 = vec2(2, 3);
   vec2  v2 = vec2(1, 4);
   bvec2 v3 = lessThan(v1, v2); //v3 contem (false, true) pois 2>1 e 1<4
   if(v3.y == false)
      gl_FragColor = vec4( 1,0,0, 1);
   else
      gl_FragColor = vec4( 0,0,1, 1);
   /* */
}
