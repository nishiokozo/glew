#define	pi	M_PI

typedef	struct
{
	float	x;	
	float	y;	
	float	z;	
	float	w;	
} vector;

class	vec2
{
public:
	union
	{
		float	x;	
		float	u;	
		float	s;	
	};
	union
	{
		float	y;	
		float	v;	
		float	t;	
	};

	vec2( const float ax, const float ay )
	{
		x = ax;
		y = ay;
	}
};


class vec3
{
    friend class vec4;

public:
	union
	{
		float	x;	
		float	r;	
	};
	union
	{
		float	y;	
		float	g;	
	};
	union
	{
		float	z;	
		float	b;	
	};
	
	vec3(){};
	vec3( float a )
	{
		x = a;
		y = a;
		z = a;
	};

	vec3( float ax, float ay, float az )
	{
		x = ax;
		y = ay;
		z = az;
	}

	vec3& operator*=( const vec3& v ) 
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		
		return *this;
	}
	vec3& operator/=( const vec3& v ) 
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		
		return *this;
	}
	vec3& operator+=( const vec3& v ) 
	{
		x += v.x;
		y += v.y;
		z += v.z;
		
		return *this;
	}

	vec3& operator-=( const vec3& v ) 
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		
		return *this;
	}

	vec3 operator-() const
	{
		vec3	ret;
		ret.x = -x;
		ret.y = -y;
		ret.z = -z;
		return ret;
	}
	vec3 operator+() const
	{
		vec3	ret;
		ret.x = x;
		ret.y = y;
		ret.z = z;
		return ret;
	}
	vec3 operator*( const float f ) const
	{
		vec3	ret;
		ret.x = x * f;
		ret.y = y * f;
		ret.z = z * f;
		return ret;
	}

	vec3 operator/( const float f ) const
	{
		float a = 1.0f / f;
		vec3	ret;
		ret.x = x * a;
		ret.y = y * a;
		ret.z = z * a;
		return ret;
	}

	vec3 operator*( const vec3& v ) const
	{
		vec3	ret;
		ret.x = x * v.x;
		ret.y = y * v.y;
		ret.z = z * v.z;
		return ret;
	}

	vec3 operator-( const vec3& v ) const
	{
		vec3	ret;
		ret.x = x - v.x;
		ret.y = y - v.y;
		ret.z = z - v.z;
		return ret;
	}

	vec3 operator+( const vec3& v ) const
	{
		vec3	ret;
		ret.x = x + v.x;
		ret.y = y + v.y;
		ret.z = z + v.z;
		return ret;
	}

	friend	vec3 operator*( const float f, const vec3& v ) 
	{
		vec3	ret;
		ret.x = f * v.x;
		ret.y = f * v.y;
		ret.z = f * v.z;
		
		return ret;
	}

	friend	vec3 operator+( const float f, const vec3& v ) 
	{
		vec3	ret;
		ret.x = f + v.x;
		ret.y = f + v.y;
		ret.z = f + v.z;
		
		return ret;
	}


	void print()
	{
		printf("vec3:%9.6f %9.6f %9.6f\n", x, y, z );
	}

	void print( const char* str )
	{
		printf("vec3:%s%9.6f %9.6f %9.6f\n", str, x, y, z );
	}

};


class vec4 : public vec3
{
public:
	union{		float	w,a;		};

	vec4(){};
	vec4( float f )
	{
		x = f;
		y = f;
		z = f;
		w = f;
	};

	vec4( float ax, float ay, float az, float aw )
	{
		x = ax;
		y = ay;
		z = az;
		w = aw;
	};
	vec4( const vec3& v, float aw )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = aw;
	};

	vec4 operator-( const vec4& v ) const
	{
		vec4	ret;
		ret.x = x - v.x;
		ret.y = y - v.y;
		ret.z = z - v.z;
		ret.w = w - v.w;
		return ret;
	}
	vec4 operator+( const vec4& v ) const
	{
		vec4	ret;
		ret.x = x + v.x;
		ret.y = y + v.y;
		ret.z = z + v.z;
		ret.w = w + v.w;
		return ret;
	}
	vec4& operator+=( const vec4& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		
		return *this;
	}
	vec4 operator*( const float f ) const
	{
		vec4	ret;
		ret.x = x * f;
		ret.y = y * f;
		ret.z = z * f;
		ret.w = w * f;
		return ret;
	}
	vec4 operator/( const float f ) const
	{
		vec4	ret;
		ret.x = x / f;
		ret.y = y / f;
		ret.z = z / f;
		ret.w = w / f;
		return ret;
	}

	friend	vec4 operator*( const float f, const vec4& v )
	{
		vec4	ret;
		ret.x = f * v.x;
		ret.y = f * v.y;
		ret.z = f * v.z;
		ret.w = f * v.w;
		
		return ret;
	}


	void print()
	{
		printf("vec4:%9.6f %9.6f %9.6f %9.6f\n", x, y, z, w );
	}

	void print( const char* str )
	{
		printf("vec4:%s%9.6f %9.6f %9.6f %9.6f\n", str, x, y, z, w );
	}

};

float	vec_dotproduct( float a[3], float b[3] );

void	vec_crossproduct( float v[3], float a[3], float b[3] );
vec3	       cross( const vec3& a, const vec3& b );

vec3	       normalize( const vec3& a );
void	vec_normalize( float v[3] );
void	vec_normalize( vec3* v );
float	       length( vec3& a );
float	vec_length( float v[3] );
float	vec_length( vec3& v );

void	vec_multiplyMatrix( float v[4], float m[16], float a[4] );

float	dot( const vec3& a, const vec3& b );

float	dot( vec4& a, vec3& b );
float	dot( vec3& a, vec4& b );
float	dot( vec4& a, vec4& b );

vec3 refract( const vec3& a, const vec3& b, float n );
vec3 reflect( const vec3& a, const vec3& b );

//float	abs( float );
float	max( float a, float b );
vec3	max( float a, const vec3& b );
vec3	min( const vec3& b, float a );
vec3	max( const vec3& b, float a );
vec3	min( float a, const vec3& b );
vec3	mix( float f, const vec3& a, const vec3& b );
float	mod( float a, float b );
float	length( vec3 a );


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

	friend	vec4 operator*( const vec4& a, const mat4& m )
	{
		vec4 ret;
//printf("a    %9.6f %9.6f %9.6f %9.6f\n", a.x, a.y, a.z, a.w);
//printf("test %9.6f %9.6f %9.6f %9.6f\n", m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0] );
//printf("test %9.6f %9.6f %9.6f %9.6f\n", m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1] );
//printf("test %9.6f %9.6f %9.6f %9.6f\n", m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2] );
//printf("test %9.6f %9.6f %9.6f %9.6f\n", m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3] );
		ret.x = a.x*m.m[0][0] + a.y*m.m[1][0] + a.z*m.m[2][0] + a.w*m.m[3][0] ;
		ret.y = a.x*m.m[0][1] + a.y*m.m[1][1] + a.z*m.m[2][1] + a.w*m.m[3][1] ;
		ret.z = a.x*m.m[0][2] + a.y*m.m[1][2] + a.z*m.m[2][2] + a.w*m.m[3][2] ;
		ret.w = a.x*m.m[0][3] + a.y*m.m[1][3] + a.z*m.m[2][3] + a.w*m.m[3][3] ;

//printf("ret  %9.6f %9.6f %9.6f %9.6f\n", ret.x, ret.y, ret.z, ret.w);

		return ret;
	}

	vec4 operator*( const vec4& a ) const
	{
		vec4 ret;
		ret.x = m[0][0]*a.x + m[0][1]*a.y + m[0][2]*a.z + m[0][3]*a.w ;
		ret.y = m[1][0]*a.x + m[1][1]*a.y + m[1][2]*a.z + m[1][3]*a.w ;
		ret.z = m[2][0]*a.x + m[2][1]*a.y + m[2][2]*a.z + m[2][3]*a.w ;
		ret.w = m[3][0]*a.x + m[3][1]*a.y + m[3][2]*a.z + m[3][3]*a.w ;
		return ret;
	}

	mat4 operator*( const mat4& a ) const
	{
		mat4 ret;
		mat_multiply( ret.m_array, m_array, a.m_array );
		return ret;
	}
	float* getArray(){ return m_array; };

	void Identity()
	{
		m[0][0] = 1.0f;	m[0][1] = 0.0f;	m[0][2] = 0.0f;	m[0][3] = 0.0f;
		m[1][0] = 0.0f;	m[1][1] = 1.0f;	m[1][2] = 0.0f;	m[1][3] = 0.0f;
		m[2][0] = 0.0f;	m[2][1] = 0.0f;	m[2][2] = 1.0f;	m[2][3] = 0.0f;
		m[3][0] = 0.0f;	m[3][1] = 0.0f;	m[3][2] = 0.0f;	m[3][3] = 1.0f;
	}
	
	void Tlanslate( vec3 scale )
	{
		m[3][0] += scale.x;
		m[3][1] += scale.y;
		m[3][2] += scale.z;
	}
	void Tlanslate( float x, float y, float z )
	{
		m[3][0] += x;
		m[3][1] += y;
		m[3][2] += z;
	}
	
	void Scale( float x, float y, float z )
	{
		m[0][0] *= x;
		m[1][1] *= y;
		m[2][2] *= z;
	}
	void Scale( vec3 scale )
	{
		m[0][0] *= scale.x;
		m[1][1] *= scale.y;
		m[2][2] *= scale.z;
	}

	void print()
	{
		printf("mat4:\n");
		printf("%9.6f %9.6f %9.6f %9.6f\n", m[0][0], m[0][1], m[0][2], m[0][3] );
		printf("%9.6f %9.6f %9.6f %9.6f\n", m[1][0], m[1][1], m[1][2], m[1][3] );
		printf("%9.6f %9.6f %9.6f %9.6f\n", m[2][0], m[2][1], m[2][2], m[2][3] );
		printf("%9.6f %9.6f %9.6f %9.6f\n", m[3][0], m[3][1], m[3][2], m[3][3] );
	}
	void print( const char* str )
	{
		printf("mat4:%s\n", str);
		printf("%9.6f %9.6f %9.6f %9.6f\n", m[0][0], m[0][1], m[0][2], m[0][3] );
		printf("%9.6f %9.6f %9.6f %9.6f\n", m[1][0], m[1][1], m[1][2], m[1][3] );
		printf("%9.6f %9.6f %9.6f %9.6f\n", m[2][0], m[2][1], m[2][2], m[2][3] );
		printf("%9.6f %9.6f %9.6f %9.6f\n", m[3][0], m[3][1], m[3][2], m[3][3] );
	}

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

vec4	mul( const vec4& a, const mat4& m );

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
