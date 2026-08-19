// Vector3.cpp: implementation of the Vector3 class.
//
//////////////////////////////////////////////////////////////////////



#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Vector3.h"
#include "../defs.h"



//----------------------------------------------------------------------
// Construction/Destruction
//----------------------------------------------------------------------
Vector3::Vector3(const Vector3& u)
{
	x = u.x; 
	y = u.y; 
	z = u.z;
}

Vector3::~Vector3()
{

}

void Vector3::set( const float vx, const float vy, const float vz )
{
	x = vx;
	y = vy;
	z = vz;						
};

void Vector3::set(const Vector3 v)
{
	x = v.x;
	y = v.y;
	z = v.z;						
};


//----------------------------------------------------------------------
// OPERATORS OPERATORS OPERATORS OPERATORS
//----------------------------------------------------------------------
Vector3& Vector3::operator-= (const Vector3& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

Vector3& Vector3::operator+= (const Vector3& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}


Vector3& Vector3::operator*= (float f)
{
	x *= f; y *= f; z *= f;
	return *this;
}

Vector3 Vector3::operator + ( const Vector3& v )
{
	Vector3 aux( x + v.x, y + v.y, z + v.z );
	return( aux );
}

//	Subtracao de vetores
Vector3 Vector3::operator - ( Vector3& v )
{
	Vector3 aux( x - v.x, y - v.y, z - v.z );
	return( aux );
}

//	Produto por escalar (float)
Vector3 Vector3::operator * ( const float s )
{
	Vector3 aux( x * s, y * s, z * s );
	return( aux );
}

//	Produto vetorial
Vector3 Vector3::operator ^ ( Vector3& v )
{
	Vector3 aux( y * v.z - z * v.y,
			   z * v.x - x * v.z,
			   x * v.y - y * v.x );
	return( aux );
}


//----------------------------------------------------------------------
// OPERACOES DE TRANFORMACAO
//----------------------------------------------------------------------
Vector3 Vector3::cross(Vector3& v)
{
	Vector3 aux(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x  );
	return aux;
}



 
//retorna entre [-1, 1] pois senao da erro no calculo do acos(ang)
float Vector3::dot(Vector3& v)
{
	float ret;
	ret = x*v.x + y*v.y + z*v.z;
	return ret;
}

float Vector3::dot2D(Vector3& v)
{
	float ret;
	ret = x*v.x + z*v.z;
	return ret;
}


/////////////////////////////////////////////////////////////
//funcao que deve ser otimizada
//calcula o angulo entre [0,2PI] e depois aplica o angulo sobre um vetor.
//Se o novo vetor for igual ao outro, o angulo esta correto, senao inverte o sinal
/////////////////////////////////////////////////////////////
float Vector3::angulo(Vector3& v)
{
	float dp, angPI ;
	
	dp = dot( v ); //dot product
	if(dp >= 1.0) dp = 1.0f;
	if(dp <=-1.0) dp =-1.0f;
	
	angPI = (float)acos( dp );

	//verifica se aplicando o angulo encontrado sobre o vetor v iguala os vetores
	Vector3 tmp(v);
	tmp.rotatey(angPI);
	if( fabs(tmp.x-x) + fabs(tmp.z-z) < 0.001 )
		return angPI;
	return -angPI; //tudo indica que o angulo eh negativo
}

float Vector3::angulo2D(Vector3& v)
{
	float dp, angPI ;
	
	dp = dot2D( v ); //dot product
	if(dp >= 1.0) dp = 1.0f;
	if(dp <=-1.0) dp =-1.0f;
	
	angPI = (float)acos( dp );

	//verifica se aplicando o angulo encontrado sobre o vetor v iguala os vetores
	Vector3 tmp(v);
	tmp.rotatey(angPI);
	if( fabs(tmp.x-x) + fabs(tmp.z-z) < 0.001 )
		return angPI;
	return -angPI; //tudo indica que o angulo eh negativo
}


float Vector3::modulo()
{
	return (float)sqrt(x*x + y*y + z*z);
}


void Vector3::normalize()
{
	float norm = (float)sqrt(x*x + y*y + z*z);

	if(norm==0.0)
	{
		printf("\n\nNormalize::Divisao por zero");
		exit(1);
	}
	x /= norm;
	y /= norm;
	z /= norm;
}

void Vector3::normalize2D()
{
	float norm = (float)sqrt(x*x + z*z);

	if(norm==0.0)
	{
		printf("\n\nNormalize::Divisao por zero");
		exit(1);
	}
	x /= norm;
	y = 0.0;
	z /= norm;
}


void Vector3::inverte()
{
	x = -x; y = -y; z = -z;
}

void Vector3::perpendicular()
{
	rotatey((float)PI2);	
}


void Vector3::rotatex(float angle)
{
	float c = (float)cos(angle);
	float s = (float)sin(angle);
	float y2 = y*c + z*s;
	float z2 = z*c - y*s;
	y = y2; z = z2;
}

void Vector3::rotatey(float angle)
{
	float c = (float)cos(angle);
	float s = (float)sin(angle);
	float x2 = x*c - z*s;
	float z2 = z*c + x*s;
	x = x2; z = z2;
}

void Vector3::rotatez(float angle)
{
	float c = (float)cos(angle);
	float s = (float)sin(angle);
	float x2 = x*c + y*s;
	float y2 = y*c - x*s;
	x = x2; y = y2;
}

float Vector3::distancia3D(Vector3& v, bool sqr)
{
	float x1, y1, z1, res;
	x1 = x - v.x;
	y1 = y - v.y;
	z1 = z - v.z;

	res = x1*x1 + y1*y1 + z1*z1;
	if(sqr==true)
		res = (float)sqrt(res);
	return res;
}

float Vector3::distancia2D(Vector3& v, bool sqr)
{
	float x1, z1, res;
	x1 = x - v.x;
	z1 = z - v.z;

	res = x1*x1 + z1*z1;
	if(sqr==true)
		res = (float)sqrt(res);
	return res;
}

//distancia do ponto P a uma reta R com extremidades (p1, p2). A reta eh transformada na
//forma L = B + tM, onde B eh p1 e M eh a direcao. (David Eberly, pg. 38)
/*float Vector3::distanciaReta(Vector3& p1, Vector3& p2)
{
	Vector3 B(p1);
	Vector3 P(x, y, z);
	Vector3 M(p2-p1);
	Vector3 diff(P-B);
	M.normalize();
	float t = M.dot(diff);
	if(t>0)
	{
		float dotMM = M.dot(M);
		if(t<dotMM)
		{
			//t = t/dotMM;
			diff = diff-(M*t);
		}
		else
		{
			//t=1;
			diff = diff-M;
		}
	}
	else
	{
		//t=0;
	}

	return sqrt(diff.dot(diff));
}/**/

//distancia do ponto P0 a uma reta R com extremidades (P1, P2). A reta eh 
// definida por um ponto P1 e um vetor diretor V = (P1P2), 
//nao necessariamente unitario. (Steinbruch pg. 191)
float Vector3::distanciaReta(Vector3& p1, Vector3& p2)
{
	Vector3 V(p2-p1);    //vetor diretor 
	Vector3 P0(x, y, z); //ponto a ser calculada a distancia da reta
	Vector3 P1P0(P0-p1); //vetor que une P1 a P0
	
	Vector3 Cross(V.cross(P1P0)); 
	
	float d = Cross.modulo() / V.modulo();

	return d;
}/**/



//p1 = ponto de partida
//p2 = ponto de chegar 
//raio = raio do circulo ao redor do ponto=(this) sendo testado 
bool Vector3::interRetaCirc(Vector3& p1, Vector3& p2, float raio)
{
	//distancia do objeto ao ator
	//1) calcula a distancia do actorDir com o centro do objeto
	float d1 = distancia2D(p1, true) - raio;
	float d2 = distanciaReta(p1, p2);
	float d3 = p1.distancia2D(p2, true);

	//printf(" %0.1f\t  ",d2);

	if(d2 < raio)
	{
		if(d3>d1)
		{
			return true;
		}
	}
	return false;
}



void Vector3::print(void)
{
	printf("\n %.2f %.2f %.2f", x, y, z);
}
