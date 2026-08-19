#ifndef __VECTOR_H__
#define __VECTOR_H__


class Vector{

public:
   float x, y;

   Vector();
   
   Vector(float _x, float _y);
   
   void set(float _x, float _y);
   
   float angleSign(const Vector& v) const;

   float dot(const Vector& v) const;
   
   void  normalize();
   
   void print();

   Vector operator - (const Vector& v);
   Vector operator + (const Vector& v);
   Vector operator * (const float f);

   float norm() const;
   
};

#endif
 
