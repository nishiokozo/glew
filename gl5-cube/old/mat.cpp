/*
#include	<iostream>
using std::cout;
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<time.h>
#include <sys/time.h>
#include <limits.h>
#include <vec.h>
#include <mat.h>
#include <quat.h>
*/
#include <stdio.h>
#include <math.h>
#include "vec.h"
#include "mat.h"


//-----------------------------------------------------------------------------
int	mat_print( matrix m )
//-----------------------------------------------------------------------------
{
	printf("-----\n" );
	printf("%9f %9f %9f %9f\n", m[M00], m[M01], m[M02], m[M03] );
	printf("%9f %9f %9f %9f\n", m[M10], m[M11], m[M12], m[M13] );
	printf("%9f %9f %9f %9f\n", m[M20], m[M21], m[M22], m[M23] );
	printf("%9f %9f %9f %9f\n", m[M30], m[M31], m[M32], m[M33] );
	return true;
}
//-----------------------------------------------------------------------------
int	mat_print( mat4& m )
//-----------------------------------------------------------------------------
{
	printf("-----\n" );
	printf("%9f %9f %9f %9f\n", m.m_array[M00], m.m_array[M01], m.m_array[M02], m.m_array[M03] );
	printf("%9f %9f %9f %9f\n", m.m_array[M10], m.m_array[M11], m.m_array[M12], m.m_array[M13] );
	printf("%9f %9f %9f %9f\n", m.m_array[M20], m.m_array[M21], m.m_array[M22], m.m_array[M23] );
	printf("%9f %9f %9f %9f\n", m.m_array[M30], m.m_array[M31], m.m_array[M32], m.m_array[M33] );
	return true;
}


//-----------------------------------------------------------------------------
void mat_invers( mat4& a, mat4& y1 )
//-----------------------------------------------------------------------------
{
	mat_invers( a.getArray(), a.getArray() );
}
//-----------------------------------------------------------------------------
void mat_invers( matrix m, matrix in )
//-----------------------------------------------------------------------------
{
//	float a[4][4]={{1,2,0,-1},{-1,1,2,0},{2,0,1,1},{1,-2,-1,1}}; //入力用の配列
	float a[4][4];
//memcpy( a, in, sizeof(float[16] ) );
	a[0][0] = in[0];
	a[0][1] = in[1]; 
	a[0][2] = in[2]; 
	a[0][3] = in[3]; 
	a[1][0] = in[4]; 
	a[1][1] = in[5]; 
	a[1][2] = in[6]; 
	a[1][3] = in[7]; 
	a[2][0] = in[8]; 
	a[2][1] = in[9]; 
	a[2][2] = in[10]; 
	a[2][3] = in[11]; 
	a[3][0] = in[12]; 
	a[3][1] = in[13]; 
	a[3][2] = in[14]; 
	a[3][3] = in[15]; 



	float inv_a[4][4]; //ここに逆行列が入る
	float buf; //一時的なデータを蓄える
	int i,j,k; //カウンタ
	int z1=4;  //配列の次数

	//単位行列を作る
	for(i=0;i<z1;i++){
	  for(j=0;j<z1;j++){
	    inv_a[i][j]=(i==j)?1.0:0.0;
	  }
	}
	//掃き出し法
	for(i=0;i<z1;i++){
	  buf=1/a[i][i];
	  for(j=0;j<z1;j++){
	    a[i][j]*=buf;
	    inv_a[i][j]*=buf;
	  }
	  for(j=0;j<z1;j++){
	    if(i!=j){
	      buf=a[j][i];
	      for(k=0;k<z1;k++){
	        a[j][k]-=a[i][k]*buf;
	        inv_a[j][k]-=inv_a[i][k]*buf;
	      }
	    }
	  }
	}
	//逆行列を出力
	int	s = 0;
	for(i=0;i<z1;i++){
	  for(j=0;j<z1;j++){
//	    printf(" %z2",inv_a[i][j]);
		m[s] = inv_a[i][j];
		s++;

	  }
//	  printf("\n");
	}

#if 0
vec_normalize( (float*)&m[0] );
vec_normalize( (float*)&m[4] );
vec_normalize( (float*)&m[8] );
#endif
}

//-----------------------------------------------------------------------------
void	mat_ortho( matrix m, float x1, float x2, float y1, float y2, float z1, float z2 )
//-----------------------------------------------------------------------------
{
	float sx =  (2.0     )/( x2 - x1 );
	float sy =  (2.0     )/( y2 - y1 );
	float sz = -(2.0     )/( z2 - z1 );
	float tx =  -(x2+x1)/(x2-x1);
	float ty =  -(y2+y1)/(y2-y1);
	float tz =  -(z2+z1)/(z2-z1);


	m[ 0] =   sx;	m[ 1] = 0.0f;	m[ 2] = 0.0f;	m[ 3] = 0.0f;
	m[ 4] = 0.0f;	m[ 5] =   sy;	m[ 6] = 0.0f;	m[ 7] = 0.0f;
	m[ 8] = 0.0f;	m[ 9] = 0.0f;	m[10] =   sz;	m[11] = 0.0f;
	m[12] =   tx;	m[13] =   ty;	m[14] =   tz;	m[15] = 1.0f;
}
//-----------------------------------------------------------------------------
void mat_perspective ( mat4& m, float fovy, float aspect, float zNear, float zFar)
//-----------------------------------------------------------------------------
{
	mat_perspective ( m.getArray(), fovy, aspect, zNear, zFar );
}
//-----------------------------------------------------------------------------
void mat_perspective (matrix m, float fovy, float aspect, float zNear, float zFar)
//-----------------------------------------------------------------------------
{
        float p = 1.0f / tanf(fovy * (M_PI/360.0));

/*
        m[0] = z2 / aspect;
        m[1] = 0.0;
        m[2] = 0.0;
        m[3] = 0.0;

        m[4] = 0.0;
        m[5] = z2;
        m[6] = 0.0;
        m[7] = 0.0;

        m[8] = 0.0;
        m[9] = 0.0;
        m[10] = (zFar + zNear) / (zNear - zFar);
        m[11] = -1.0;

        m[12] = 0.0;
        m[13] = 0.0;
        m[14] = 2.0 * zFar * zNear / (zNear - zFar);
        m[15] = 0.0;
*/

/*
	float	p = {
        z2 / aspect,	0.0,	0.0,									 0.0,
        0.0,		z2,		0.0,									 0.0,
        0.0,        0.0,    (zFar + zNear) / (zNear - zFar),        -1.0,
 		0.0,        0.0,	2.0 * zFar * zNear / (zNear - zFar),	 0.0
	};

	mat_copy( m, p );
*/
	float	z2 = zFar;
	float	z1 = zNear;


        m[ 0] = p / aspect; m[ 1] = 0.0;	m[ 2] = 0.0;        			m[ 3] =  0.0;
        m[ 4] = 0.0;        m[ 5] = p;    	m[ 6] = 0.0;        			m[ 7] =  0.0;
        m[ 8] = 0.0;        m[ 9] = 0.0;  	m[10] = (z2+z1)         / (z1-z2); 	m[11] = -1.0;
        m[12] = 0.0;    	m[13] = 0.0;	m[14] = (2.0 * z2 * z1) / (z1-z2);  m[15] =  0.0;


/*
float w = z2 / aspect;
float h = z2;
float c = (zFar + zNear) / (zNear - zFar);
float d = 2.0 * zFar * zNear / (zNear - zFar);
          w,	0.0,	 0.0,	 0.0,
        0.0,	  h,	 0.0,	 0.0,
        0.0,    0.0,       c,      d,
 		0.0,    0.0,	-1.0,	 0.0
	};

	mat_copy( m, p );
*/

}
//-----------------------------------------------------------------------------
void mat_ray_perspective (matrix m, float fovy, float aspect )
//-----------------------------------------------------------------------------
{
        float	p = 1.0f / tanf(fovy * (M_PI/360.0));
		float	ofs_x = 0.0;	//x	offset-x
		float	ofs_y = 0.0;	//y	offset-y
		float	ofs_z = -p;		//z	offset-z


        m[ 0] = 1.0* aspect;m[ 1] = 0.0;    m[ 2] = 0.0;    m[ 3] = 0.0;
        m[ 4] = 0.0;  		m[ 5] = 1.0;    m[ 6] = 0.0;    m[ 7] = 0.0;
        m[ 8] = 0.0;        m[ 9] = 0.0;    m[10] = 1.0;    m[11] = 0.0;
        m[12] = ofs_x;      m[13] = ofs_y;	m[14] = ofs_z;	m[15] = 0.0;

}

//-----------------------------------------------------------------------------
void mat_glFrustumf ( matrix m,  float x1, float x2, float y1, float y2, float z1, float z2)
//-----------------------------------------------------------------------------
{
    m[ 0] = (2*z1) / (x2-x1);	m[ 1] = 0.0;         	m[ 2] = 0.0;        		m[ 3] = 0.0;
    m[ 4] = 0.0;       	 	m[ 5] = (2*z1) / (y2-y1);	m[ 6] = 0.0;        		m[ 7] = 0.0;
    m[ 8] = (x2+x1) / (x2-x1);	m[ 9] = (y2+y1) / (y2-y1); 	m[10] = -(z2+z1) / (z2-z1);     m[11] =-1.0;
    m[12] = 0.0;         	m[13] = 0.0;         	m[14] = -(2*z1*z2) / (z2-z1);   m[15] = 0.0;

}
//-----------------------------------------------------------------------------
void	mat_frustum( matrix m, float x1, float x2, float y1, float y2, float z1, float z2 )
//-----------------------------------------------------------------------------
{
	float sx =       (2.0f * z1) / (x2-x1);
	float sy =       (2.0f * z1) / (y2-y1);
	float mx =           (x2+x1) / (x2-x1);
	float my =           (y2+y1) / (y2-y1);
	float sz =          -(z2+z1) / (z2-z1);
	float w  = -(2.0 * z2 * z1 ) / (z2-z1);
	m[M00] =   sx;	m[M01] = 0.0f;	m[M02] =  mx;	m[M03] = 0.0f;
	m[M10] = 0.0f;	m[M11] =   sy;	m[M12] =  my;	m[M13] = 0.0f;
	m[M20] = 0.0f;	m[M21] = 0.0f;	m[M22] =  sz;	m[M23] =    w;
	m[M30] = 0.0f;	m[M31] = 0.0f;	m[M32] =-1.0f;	m[M33] = 0.0f;
}
//-----------------------------------------------------------------------------
void	mat_Frustum( mat4& m, float x1, float x2, float y1, float y2, float z1, float z2 )
//-----------------------------------------------------------------------------
{
	float* a = m.m_array;


	float sx =    2.0f / (x2-x1);
	float sy =    2.0f / (y2-y1);
	float sz =    2.0f / (z2-z1);
//	float tx = (x2+x1) / (x2-x1);
//	float ty = (y2+y1) / (y2-y1);
//	float tz = (z2+z1) / (z2-z1);
	
	float w = 1.0f/z1;
	
sx = 1.0f;
sy = 1.0f;
sz = 1.0f;
w = 1.0f/2.0f;


	a[M00] =   sx;	a[M01] = 0.0f;	a[M02] = 0.0f;	a[M03] = 0.0f;
	a[M10] = 0.0f;	a[M11] =   sy;	a[M12] = 0.0f;	a[M13] = 0.0f;
	a[M20] = 0.0f;	a[M21] = 0.0f;	a[M22] =   sz;	a[M23] =    w;
	a[M30] = 0.0f;	a[M31] = 0.0f;	a[M32] = 0.0f;	a[M33] = 0.0f;

}
//-----------------------------------------------------------------------------
void	mat_Ortho( mat4& m, float x1, float x2, float y1, float y2, float z1, float z2 )
//-----------------------------------------------------------------------------
{
	float* a = m.m_array;

	float sx =    2.0f / (x2-x1);
	float sy =    2.0f / (y2-y1);
	float sz =    2.0f / (z2-z1);
	float tx = (x2+x1) / (x2-x1);
	float ty = (y2+y1) / (y2-y1);
	float tz = (z2+z1) / (z2-z1);
	a[M00] =   sx;	a[M01] = 0.0f;	a[M02] = 0.0f;	a[M03] = 0.0f;
	a[M10] = 0.0f;	a[M11] =   sy;	a[M12] = 0.0f;	a[M13] = 0.0f;
	a[M20] = 0.0f;	a[M21] = 0.0f;	a[M22] =   sz;	a[M23] = 0.0f;
	a[M30] =   tx;	a[M31] =   ty;	a[M32] =   tz;	a[M33] = 1.0f;
}
//-----------------------------------------------------------------------------
void	mat_Ortho( matrix m, float x1, float x2, float y1, float y2, float z1, float z2 )
//-----------------------------------------------------------------------------
{

	float sx =    2.0f / (x2-x1);
	float sy =    2.0f / (y2-y1);
	float sz =    2.0f / (z2-z1);
	float tx = (x2+x1) / (x2-x1);
	float ty = (y2+y1) / (y2-y1);
	float tz = (z2+z1) / (z2-z1);
	m[M00] =   sx;	m[M01] = 0.0f;	m[M02] = 0.0f;	m[M03] = 0.0f;
	m[M10] = 0.0f;	m[M11] =   sy;	m[M12] = 0.0f;	m[M13] = 0.0f;
	m[M20] = 0.0f;	m[M21] = 0.0f;	m[M22] =   sz;	m[M23] = 0.0f;
	m[M30] =   tx;	m[M31] =   ty;	m[M32] =   tz;	m[M33] = 1.0f;
}

//-----------------------------------------------------------------------------
void	mat_multiply( matrix m, const matrix a, const matrix y1 )
//-----------------------------------------------------------------------------
{

	float	a00 = a[M00];
	float	a01 = a[M01];
	float	a02 = a[M02];
	float	a03 = a[M03];
	float	a10 = a[M10];
	float	a11 = a[M11];
	float	a12 = a[M12];
	float	a13 = a[M13];
	float	a20 = a[M20];
	float	a21 = a[M21];
	float	a22 = a[M22];
	float	a23 = a[M23];
	float	a30 = a[M30];
	float	a31 = a[M31];
	float	a32 = a[M32];
	float	a33 = a[M33];

	float	b00 = y1[M00];
	float	b01 = y1[M01];
	float	b02 = y1[M02];
	float	b03 = y1[M03];
	float	b10 = y1[M10];
	float	b11 = y1[M11];
	float	b12 = y1[M12];
	float	b13 = y1[M13];
	float	b20 = y1[M20];
	float	b21 = y1[M21];
	float	b22 = y1[M22];
	float	b23 = y1[M23];
	float	b30 = y1[M30];
	float	b31 = y1[M31];
	float	b32 = y1[M32];
	float	b33 = y1[M33];

	m[M00] = a00 * b00 +  a01 * b10 +  a02 * b20 +  a03 * b30;
	m[M01] = a00 * b01 +  a01 * b11 +  a02 * b21 +  a03 * b31;
	m[M02] = a00 * b02 +  a01 * b12 +  a02 * b22 +  a03 * b32;
	m[M03] = a00 * b03 +  a01 * b13 +  a02 * b23 +  a03 * b33;

	m[M10] = a10 * b00 +  a11 * b10 +  a12 * b20 +  a13 * b30;
	m[M11] = a10 * b01 +  a11 * b11 +  a12 * b21 +  a13 * b31;
	m[M12] = a10 * b02 +  a11 * b12 +  a12 * b22 +  a13 * b32;
	m[M13] = a10 * b03 +  a11 * b13 +  a12 * b23 +  a13 * b33;

	m[M20] = a20 * b00 +  a21 * b10 +  a22 * b20 +  a23 * b30;
	m[M21] = a20 * b01 +  a21 * b11 +  a22 * b21 +  a23 * b31;
	m[M22] = a20 * b02 +  a21 * b12 +  a22 * b22 +  a23 * b32;
	m[M23] = a20 * b03 +  a21 * b13 +  a22 * b23 +  a23 * b33;

	m[M30] = a30 * b00 +  a31 * b10 +  a32 * b20 +  a33 * b30;
	m[M31] = a30 * b01 +  a31 * b11 +  a32 * b21 +  a33 * b31;
	m[M32] = a30 * b02 +  a31 * b12 +  a32 * b22 +  a33 * b32;
	m[M33] = a30 * b03 +  a31 * b13 +  a32 * b23 +  a33 * b33;
}
//-----------------------------------------------------------------------------
void	mat_sub( matrix m, matrix a, matrix y1 )
//-----------------------------------------------------------------------------
{
	m[M00] = a[M00]-y1[M00];
	m[M01] = a[M01]-y1[M01];
	m[M02] = a[M02]-y1[M02];
	m[M03] = a[M03]-y1[M03];

	m[M10] = a[M10]-y1[M10];
	m[M11] = a[M11]-y1[M11];
	m[M12] = a[M12]-y1[M12];
	m[M13] = a[M13]-y1[M13];

	m[M20] = a[M20]-y1[M20];
	m[M21] = a[M21]-y1[M21];
	m[M22] = a[M22]-y1[M22];
	m[M23] = a[M23]-y1[M23];

	m[M30] = a[M30]-y1[M30];
	m[M31] = a[M31]-y1[M31];
	m[M32] = a[M32]-y1[M32];
	m[M33] = a[M33]-y1[M33];
}
//-----------------------------------------------------------------------------
void	mat_add( matrix m, matrix a, matrix y1 )
//-----------------------------------------------------------------------------
{
	m[M00] = a[M00]+y1[M00];
	m[M01] = a[M01]+y1[M01];
	m[M02] = a[M02]+y1[M02];
	m[M03] = a[M03]+y1[M03];

	m[M10] = a[M10]+y1[M10];
	m[M11] = a[M11]+y1[M11];
	m[M12] = a[M12]+y1[M12];
	m[M13] = a[M13]+y1[M13];

	m[M20] = a[M20]+y1[M20];
	m[M21] = a[M21]+y1[M21];
	m[M22] = a[M22]+y1[M22];
	m[M23] = a[M23]+y1[M23];

	m[M30] = a[M30]+y1[M30];
	m[M31] = a[M31]+y1[M31];
	m[M32] = a[M32]+y1[M32];
	m[M33] = a[M33]+y1[M33];
}
//-----------------------------------------------------------------------------
void	mat_div( matrix m, float z2 )
//-----------------------------------------------------------------------------
{
	m[M00] /= z2;
	m[M01] /= z2;
	m[M02] /= z2;
	m[M03] /= z2;

	m[M10] /= z2;
	m[M11] /= z2;
	m[M12] /= z2;
	m[M13] /= z2;

	m[M20] /= z2;
	m[M21] /= z2;
	m[M22] /= z2;
	m[M23] /= z2;

	m[M30] /= z2;
	m[M31] /= z2;
	m[M32] /= z2;
	m[M33] /= z2;
}
//-----------------------------------------------------------------------------
void	mat_copy( matrix m, float a[16] )
//-----------------------------------------------------------------------------
{
//	memcpy( m, a, sizeof(float[16]) );
	m[0] = a[0];
	m[1] = a[1]; 
	m[2] = a[2]; 
	m[3] = a[3]; 
	m[4] = a[4]; 
	m[5] = a[5]; 
	m[6] = a[6]; 
	m[7] = a[7]; 
	m[8] = a[8]; 
	m[9] = a[9]; 
	m[10] = a[10]; 
	m[11] = a[11]; 
	m[12] = a[12]; 
	m[13] = a[13]; 
	m[14] = a[14]; 
	m[15] = a[15]; 

}
//-----------------------------------------------------------------------------
void	mat_identity( mat4& m )
//-----------------------------------------------------------------------------
{
	mat_identity( m.getArray() );
}
//-----------------------------------------------------------------------------
void	mat_identity( matrix m )
//-----------------------------------------------------------------------------
{
	m[M00] = 1.0f;	m[M01] = 0.0f;	m[M02] = 0.0f;	m[M03] = 0.0f;
	m[M10] = 0.0f;	m[M11] = 1.0f;	m[M12] = 0.0f;	m[M13] = 0.0f;
	m[M20] = 0.0f;	m[M21] = 0.0f;	m[M22] = 1.0f;	m[M23] = 0.0f;
	m[M30] = 0.0f;	m[M31] = 0.0f;	m[M32] = 0.0f;	m[M33] = 1.0f;
}
//-----------------------------------------------------------------------------
void	mat_transpose( matrix m, float a[16] )
//-----------------------------------------------------------------------------
{
	float	a00 = a[M00];
	float	a01 = a[M01];
	float	a02 = a[M02];
	float	a03 = a[M03];
	float	a10 = a[M10];
	float	a11 = a[M11];
	float	a12 = a[M12];
	float	a13 = a[M13];
	float	a20 = a[M20];
	float	a21 = a[M21];
	float	a22 = a[M22];
	float	a23 = a[M23];
	float	a30 = a[M30];
	float	a31 = a[M31];
	float	a32 = a[M32];
	float	a33 = a[M33];

	m[M00] = a00;
	m[M01] = a10;
	m[M02] = a20;
	m[M03] = a30;

	m[M10] = a01;
	m[M11] = a11;
	m[M12] = a21;
	m[M13] = a31;

	m[M20] = a02;
	m[M21] = a12;
	m[M22] = a22;
	m[M23] = a32;

	m[M30] = a03;
	m[M31] = a13;
	m[M32] = a23;
	m[M33] = a33;
}
//-----------------------------------------------------------------------------
void	mat_getScale( matrix m, float sx, float sy, float sz  )
//-----------------------------------------------------------------------------
{
	m[M00] =   sx;	m[M01] = 0.0f;	m[M02] = 0.0f;	m[M03] = 0.0f;
	m[M10] = 0.0f;	m[M11] =   sy;	m[M12] = 0.0f;	m[M13] = 0.0f;
	m[M20] = 0.0f;	m[M21] = 0.0f;	m[M22] =   sz;	m[M23] = 0.0f;
	m[M30] = 0.0f;	m[M31] = 0.0f;	m[M32] = 0.0f;	m[M33] = 1.0f;
}
//-----------------------------------------------------------------------------
void	mat_getTranslate( matrix m, float x, float y, float z )
//-----------------------------------------------------------------------------
{
	m[M00] = 1.0f;	m[M01] = 0.0f;	m[M02] = 0.0f;	m[M03] = 0.0f;
	m[M10] = 0.0f;	m[M11] = 1.0f;	m[M12] = 0.0f;	m[M13] = 0.0f;
	m[M20] = 0.0f;	m[M21] = 0.0f;	m[M22] = 1.0f;	m[M23] = 0.0f;
	m[M30] =   x;	m[M31] =   y;	m[M32] =   z;	m[M33] = 1.0f;
}
//-----------------------------------------------------------------------------
void	mat_getRotateX( matrix m, float th )
//-----------------------------------------------------------------------------
{
	float	c = cos(th);
	float	s = sin(th);

	m[M00] = 1.0f;	m[M01] = 0.0f;	m[M02] = 0.0f;	m[M03] = 0.0f;
	m[M10] = 0.0f;	m[M11] =   c;	m[M12] =  -s;	m[M13] = 0.0f;
	m[M20] = 0.0f;	m[M21] =   s;	m[M22] =   c;	m[M23] = 0.0f;
	m[M30] = 0.0f;	m[M31] = 0.0f;	m[M32] = 0.0f;	m[M33] = 1.0f;
}
//-----------------------------------------------------------------------------
void	mat_scaling( matrix m, float sx, float sy, float sz  )
//-----------------------------------------------------------------------------
{
	matrix a;
	mat_getScale( a, sx, sy, sz  );
	mat_multiply( m, m, a );
}
//-----------------------------------------------------------------------------
void	mat_scaling( matrix m, const vec3& vecScale  )
//-----------------------------------------------------------------------------
{
	matrix a;
	mat_getScale( a, vecScale.x, vecScale.y, vecScale.z  );
	mat_multiply( m, m, a );
}
//-----------------------------------------------------------------------------
void	mat_scaling( mat4& m, const vec3& vecScale  )
//-----------------------------------------------------------------------------
{
	mat_scaling( m.getArray(), vecScale.x, vecScale.y, vecScale.z  );
}
//-----------------------------------------------------------------------------
void	mat_scaling( mat4& m, float sx, float sy, float sz   )
//-----------------------------------------------------------------------------
{
	mat_scaling( m.getArray(), sx, sy, sz  );
}
//-----------------------------------------------------------------------------
void	mat_translate( mat4& m, float sx, float sy, float sz   )
//-----------------------------------------------------------------------------
{
	mat_translate( m.getArray(), sx, sy, sz  );
}
//-----------------------------------------------------------------------------
void	mat_translate( matrix m, float x, float y, float z )
//-----------------------------------------------------------------------------
{
	m[M30] += x;
	m[M31] += y;
	m[M32] += z;
}

//-----------------------------------------------------------------------------
void	mat_translate( mat4& m, vec3 v )
//-----------------------------------------------------------------------------
{
	mat_translate( m.getArray(), v );
}
//-----------------------------------------------------------------------------
void	mat_translate( matrix m, vec3 v )
//-----------------------------------------------------------------------------
{
	m[M30] += v.x;
	m[M31] += v.y;
	m[M32] += v.z;
}
//-----------------------------------------------------------------------------
void	mat_rotateX( mat4& m, float th )
//-----------------------------------------------------------------------------
{
	mat_rotateX( m.getArray(), th );
}
//-----------------------------------------------------------------------------
void	mat_rotateY( mat4& m, float th )
//-----------------------------------------------------------------------------
{
	mat_rotateY( m.getArray(), th );
}
//-----------------------------------------------------------------------------
void	mat_rotateZ( mat4& m, float th )
//-----------------------------------------------------------------------------
{
	mat_rotateZ( m.getArray(), th );
}
//-----------------------------------------------------------------------------
void	mat_rotateX( matrix m, float th )
//-----------------------------------------------------------------------------
{
	matrix a;
	mat_getRotateX(a,th);

	mat_multiply( m, m, a );
}
//-----------------------------------------------------------------------------
void	mat_rotateY( matrix m, float th )
//-----------------------------------------------------------------------------
{
	matrix a;
	mat_getRotateY(a,th);

	mat_multiply( m, m, a );
}
//-----------------------------------------------------------------------------
void	mat_rotateZ( matrix m, float th )
//-----------------------------------------------------------------------------
{
	matrix a;
	mat_getRotateZ(a,th);

	mat_multiply( m, m, a );
}
//-----------------------------------------------------------------------------
void	mat_getRotateY( matrix m, float th )
//-----------------------------------------------------------------------------
{
	float	c = cos(th);
	float	s = sin(th);

	m[M00] =   c;	m[M01] = 0.0f;	m[M02] =  -s;	m[M03] = 0.0f;
	m[M10] = 0.0f;	m[M11] = 1.0f;	m[M12] = 0.0f;	m[M13] = 0.0f;
	m[M20] =   s;	m[M21] = 0.0f;	m[M22] =   c;	m[M23] = 0.0f;
	m[M30] = 0.0f;	m[M31] = 0.0f;	m[M32] = 0.0f;	m[M33] = 1.0f;
}
//-----------------------------------------------------------------------------
void	mat_rotateZ( matrix m, float a[16], float th )
//-----------------------------------------------------------------------------
{
	float	c = cos(th);
	float	s = sin(th);

	m[M00] =   c;	m[M01] =   -s;	m[M02] = 0.0f;	m[M03] = 0.0f;
	m[M10] =   s;	m[M11] =    c;	m[M12] = 0.0f;	m[M13] = 0.0f;
	m[M20] = 0.0f;	m[M21] = 0.0f;	m[M22] = 1.0f;	m[M23] = 0.0f;
	m[M30] = 0.0f;	m[M31] = 0.0f;	m[M32] = 0.0f;	m[M33] = 1.0f;

	mat_multiply( m, a, m );
}
//-----------------------------------------------------------------------------
void	mat_getRotateZ( matrix m, float th )
//-----------------------------------------------------------------------------
{
	float	c = cos(th);
	float	s = sin(th);

	m[M00] =   c;	m[M01] =   -s;	m[M02] = 0.0f;	m[M03] = 0.0f;
	m[M10] =   s;	m[M11] =    c;	m[M12] = 0.0f;	m[M13] = 0.0f;
	m[M20] = 0.0f;	m[M21] = 0.0f;	m[M22] = 1.0f;	m[M23] = 0.0f;
	m[M30] = 0.0f;	m[M31] = 0.0f;	m[M32] = 0.0f;	m[M33] = 1.0f;
}
//-----------------------------------------------------------------------------
void	mat_copyMatrix( matrix m, matrix a )
//-----------------------------------------------------------------------------
{
	m[M00] = a[M00];
	m[M01] = a[M01];
	m[M02] = a[M02];
	m[M03] = a[M03];
	m[M10] = a[M10];
	m[M11] = a[M11];
	m[M12] = a[M12];
	m[M13] = a[M13];
	m[M20] = a[M20];
	m[M21] = a[M21];
	m[M22] = a[M22];
	m[M23] = a[M23];
	m[M30] = a[M30];
	m[M31] = a[M31];
	m[M32] = a[M32];
	m[M33] = a[M33];

}
