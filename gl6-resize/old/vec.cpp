#include <math.h>
#include "vec.h"

float	abs( float f )
{
	if ( f < 0 ) return -f;
	return f;
}

//-----------------------------------------------------------------------------
float	dot( vec3& a, vec3& b )
//-----------------------------------------------------------------------------
{
	return a.x * b.x + a.y * b.y +a.z * b.z ;
}
//-----------------------------------------------------------------------------
float	dot( vec4& a, vec3& b )
//-----------------------------------------------------------------------------
{
	return a.x * b.x + a.y * b.y +a.z * b.z ;
}
//-----------------------------------------------------------------------------
float	dot( vec3& a, vec4& b )
//-----------------------------------------------------------------------------
{
	return a.x * b.x + a.y * b.y +a.z * b.z ;
}
//-----------------------------------------------------------------------------
float	dot( vec4& a, vec4& b )
//-----------------------------------------------------------------------------
{
	return a.x * b.x + a.y * b.y +a.z * b.z ;
}
//-----------------------------------------------------------------------------
float	vec_dotproduct( float a[3], float b[3] )
//-----------------------------------------------------------------------------
{
	return a[0] * b[0] + a[1] * b[1] +a[2] * b[2] ;
}
//-----------------------------------------------------------------------------
vec3	cross( const vec3& a, const vec3& b )
//-----------------------------------------------------------------------------
{
	vec3 v; 
	v.x = a.y*b.z-a.z*b.y;
	v.y = a.z*b.x-a.x*b.z;
	v.z = a.x*b.y-a.y*b.x;

	return v;
}
//-----------------------------------------------------------------------------
float	length( vec3& a )
//-----------------------------------------------------------------------------
{
	float	l = sqrtf( a.x*a.x + a.y*a.y + a.z*a.z );

	return	l;
}

//-----------------------------------------------------------------------------
vec3	normalize( vec3& a )
//-----------------------------------------------------------------------------
{
	vec3 v; 
	float	l = 1.0f / sqrtf( a.x*a.x + a.y*a.y + a.z*a.z );
	
	v.x = a.x * l;
	v.y = a.y * l;
	v.z = a.z * l;

	return	v;
}
//-----------------------------------------------------------------------------
void	vec_crossproduct( float v[3], float a[3], float b[3] )
//-----------------------------------------------------------------------------
{
	v[0] = a[1]*b[2]-a[2]*b[1];
	v[1] = a[2]*b[0]-a[0]*b[2];
	v[2] = a[0]*b[1]-a[1]*b[0];
}
//-----------------------------------------------------------------------------
float	vec_length( float v[3] )
//-----------------------------------------------------------------------------
{
	return sqrtf( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
}
//-----------------------------------------------------------------------------
float	vec_length( vec3& v )
//-----------------------------------------------------------------------------
{
	return sqrtf( v.x*v.x + v.y*v.y + v.z*v.z );
}
//-----------------------------------------------------------------------------
void	vec_normalize( vec3* v )
//-----------------------------------------------------------------------------
{
	float	l = 1 / sqrtf( v->x*v->x + v->y*v->y + v->z*v->z );
	
	
	v->x *= l;
	v->y *= l;
	v->z *= l;
}
//-----------------------------------------------------------------------------
void	vec_normalize( float v[3] )
//-----------------------------------------------------------------------------
{
	float	l = 1 / sqrtf( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
	
	
	v[0] *= l;
	v[1] *= l;
	v[2] *= l;
}
/*
//-----------------------------------------------------------------------------
void	vnormalize( vec3& a )
//-----------------------------------------------------------------------------
{
	float	l = 1.0f / sqrtf( a.x*a.x + a.y*a.y + a.z*a.z );
	
	a.x = a.x * l;
	a.y = a.y * l;
	a.z = a.z * l;
}
*/

//-----------------------------------------------------------------------------
void	vec_multiplyMatrix( float v[4], float m[16], float a[4] )
//-----------------------------------------------------------------------------
{

	float	m00 = m[ 0];
	float	m01 = m[ 1];
	float	m02 = m[ 2];
	float	m03 = m[ 3];
	float	m10 = m[ 4];
	float	m11 = m[ 5];
	float	m12 = m[ 6];
	float	m13 = m[ 7];
	float	m20 = m[ 8];
	float	m21 = m[ 9];
	float	m22 = m[10];
	float	m23 = m[11];
	float	m30 = m[12];
	float	m31 = m[13];
	float	m32 = m[14];
	float	m33 = m[15];

	float	a0 = a[0];
	float	a1 = a[1];
	float	a2 = a[2];
	float	a3 = a[3];

	v[0] = m00 * a0 +  m01 * a1 +  m02 * a2 +  m03 * a3;
	v[1] = m10 * a0 +  m11 * a1 +  m12 * a2 +  m13 * a3;
	v[2] = m20 * a0 +  m21 * a1 +  m22 * a2 +  m23 * a3;
	v[3] = m30 * a0 +  m31 * a1 +  m32 * a2 +  m33 * a3;

}
