#define	RAD(n)	((n)*M_PI/180.0f)
#define	DEG(n)	((n)*180.0f/M_PI)
#define	M00	0
#define	M01	1
#define	M02	2
#define	M03	3
#define	M10	4
#define	M11	5
#define	M12	6
#define	M13	7
#define	M20	8
#define	M21	9
#define	M22	10
#define	M23	11
#define	M30	12
#define	M31	13
#define	M32	14
#define	M33	15

#define	rad(n) ((n)*M_PI/180.0f)

typedef	float	matrix[16];
void	mat_multiply( matrix m, const float* a, const float* b );

//---
class	mat4
{
public:
	union
	{
		float	m_array[16];
		float	m[4][4];
	};
	mat4()
	{
	}

	mat4(
		float m00, float m01, float m02, float m03, 
		float m10, float m11, float m12, float m13, 
		float m20, float m21, float m22, float m23, 
		float m30, float m31, float m32, float m33 
	)
	{
		m_array[ 0] = m00;
		m_array[ 1] = m01;
		m_array[ 2] = m02;
		m_array[ 3] = m03;
		m_array[ 4] = m10;
		m_array[ 5] = m11;
		m_array[ 6] = m12;
		m_array[ 7] = m13;
		m_array[ 8] = m20;
		m_array[ 9] = m21;
		m_array[10] = m22;
		m_array[11] = m23;
		m_array[12] = m30;
		m_array[13] = m31;
		m_array[14] = m32;
		m_array[15] = m33;
	}

	mat4 operator*( const mat4& a ) const
	{
		mat4 ret;
		mat_multiply( ret.m_array, m_array, a.m_array );
		return ret;
	}
	float* getArray(){ return m_array; };
};


void	mat_Frustum( mat4& m, float l, float r, float b, float t, float n, float f );
void	mat_Ortho( matrix m, float l, float r, float b, float t, float n, float f );

int	mat_print( mat4& m );
int	mat_print( matrix m );
void	mat_frustum( matrix m, float l, float r, float b, float t, float n, float f );
void	mat_translate( mat4& m, float x, float y, float z );
void	mat_translate( matrix m, float x, float y, float z );
void	mat_translate( matrix m, vec3 vec );
void	mat_translate( mat4& m, vec3 vec );
void	mat_rotateX( matrix m, float th );
void	mat_rotateY( matrix m, float th );
void	mat_rotateZ( matrix m, float th );
void	mat_rotateX( mat4& m, float th );
void	mat_rotateY( mat4& m, float th );
void	mat_rotateZ( mat4& m, float th );
void	mat_scaling( mat4& m, float sx, float sy, float sz  );
void	mat_scaling( matrix m, float sx, float sy, float sz  );
void	mat_scaling( matrix m, const vec3& vecScale  );
void	mat_scaling( mat4& m, const vec3& vecScale  );
void	mat_transpose( matrix m, float a[16] );

void	mat_copy( matrix m, float a[16] );

void 	mat_invers( mat4& a, mat4& b );
void	mat_invers ( matrix out, matrix in );
void	mat_identity( matrix m );
void	mat_identity( mat4& m );

void	mat_getTranslate( matrix m, float x, float y, float z );
void	mat_getRotateX( matrix m, float th );
void	mat_getRotateY( matrix m, float th );
void	mat_getRotateZ( matrix m, float th );
void	mat_getScale( matrix m, float sx, float sy, float sz  );

float*	mat_identity();

void 	mat_glFrustumf ( matrix m,  float left, float right, float bottom, float top, float near, float far);
void 	mat_perspective ( mat4& m, float fovy, float aspect, float zNear, float zFar);
void 	mat_perspective (matrix m, float fovy, float aspect, float zNear, float zFar);
void 	mat_invers ( matrix m, matrix in );
void	mat_add( matrix m, float* a, float* b );
void	mat_sub( matrix m, float* a, float* b );
void	mat_div( matrix m, float f );

// for raytrace
void mat_ray_perspective (matrix m, float fovy, float aspect );
void	mat_otrho( matrix m, float l, float r, float b, float t, float n, float f );
