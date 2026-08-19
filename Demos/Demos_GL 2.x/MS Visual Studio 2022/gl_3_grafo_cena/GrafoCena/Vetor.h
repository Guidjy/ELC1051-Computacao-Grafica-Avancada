#ifndef __VETOR_H
#define __VETOR_H

#include <math.h>

class vetor
{
public:
   float x, y, z;
   
   vetor( void )
   {
      x = y = z = 0.0;                  
   };
   
   vetor( const float tx, const float ty, const float tz )
   {
      x = tx;
      y = ty;
      z = tz;                  
   };

//   construtor de copia
   vetor( const vetor& v )
   {
      this->x = v.x;
      this->y = v.y;
      this->z = v.z;                  
   };

    void setValor( const float tx, const float ty, const float tz )
   {
      x = tx;
      y = ty;
      z = tz;                  
   };

    //inicializacao de vetor
   void setValor(const vetor v)
   {
      x = v.x;
      y = v.y;
      z = v.z;                  
   };

//   Soma de vetores
   inline vetor operator + ( const vetor v )
   {
      vetor aux( x + v.x, y + v.y, z + v.z );
      return( aux );
   }

//   Subtracao de vetores
   inline vetor operator - ( const vetor v )
   {
      vetor aux( x - v.x, y - v.y, z - v.z );
      return( aux );
   }

//   Produto por escalar (float)
   inline vetor operator * ( const float s )
   {
      vetor aux( x * s, y * s, z * s );
      return( aux );
   }

//   Divisao por escalar
   inline vetor  operator / ( const float s )
   {
      vetor aux( x / s, y / s, z / s );
      return( aux );
   }

//   Produto escalar
   inline float operator * ( const vetor v )
   {
      float aux = x * v.x + y * v.y + z * v.z;
      return( aux );
   }

//   Produto vetorial
   inline vetor operator ^ ( vetor& v )
   {
      vetor aux( y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x );
      return( aux );
   }

//  norma do vetor
   float norma( void )
   {
      return (float)( sqrt( x * x + y * y + z * z ) );
   }

   float distancia( vetor v )
   {
      return( (*this - v).norma() );
   }

//   normaliza o vetor
   vetor normalize( void )
   {
      return( (*this) / (this->norma()) );
   }

   // normaliza o vetor
   vetor extende(float val)
   {
      return ((*this) * (val) );
   }
   
   // faz a inversao do vetor normal. Usada no cado de FRONT_FACE = GL_CW
   void inverte()
   {
      x = -x;
      y = -y;
      z = -z;
   }

   void print()
   {
      printf("\n%f %f %f ", x, y, z);
   }
};

#endif
