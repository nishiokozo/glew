float	abs( float );

typedef	struct
{
	float	x;	
	float	y;	
	float	z;	
	float	w;	
} vector;
#if 0
typedef	struct
{
	float	x;	
	float	y;	
} vec2;
#else
 #if 0
typedef	struct
{
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
} vec2;
 #else
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
 #endif
#endif


#if 0
typedef	struct
{
	float	x;	
	float	y;	
	float	z;	
} vec3;
#else
class vec3
{
public:
	float	x;	
	float	y;	
	float	z;	

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
	};

	vec3 operator-() const
	{
		vec3	ret;
		ret.x = -x;
		ret.y = -y;
		ret.z = -z;
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

	vec3::vec3 operator-( const vec3& v ) const
	{
		vec3	ret;
		ret.x = x - v.x;
		ret.y = y - v.y;
		ret.z = z - v.z;
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

/*
	vec3& operator=( const vec3& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		
		return *this;
	}
*/
};
#endif

#if 0
typedef	struct
{
	float	x;	
	float	y;	
	float	z;	
	float	w;	
} vec4;
#else
class vec4
{
public:
	union{		float	x,r;		};
	union{		float	y,g;		};
	union{		float	z,b;		};
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
	vec4( vec3 v, float aw )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = aw;
	};

	vec4::vec4 operator-( const vec4& v ) const
	{
		vec4	ret;
		ret.x = x - v.x;
		ret.y = y - v.y;
		ret.z = z - v.z;
		ret.w = w - v.w;
		return ret;
	}
	vec4::vec4 operator+( const vec4& v ) const
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


};
#endif

float	vec_dotproduct( float a[3], float b[3] );

void	vec_crossproduct( float v[3], float a[3], float b[3] );
vec3	       cross( const vec3& a, const vec3& b );

vec3	       normalize( vec3& a );
void	vec_normalize( float v[3] );
void	vec_normalize( vec3* v );
float	       length( vec3& a );
float	vec_length( float v[3] );
float	vec_length( vec3& v );

void	vec_multiplyMatrix( float v[4], float m[16], float a[4] );

float	dot( vec3& a, vec3& b );
float	dot( vec4& a, vec3& b );
float	dot( vec3& a, vec4& b );
float	dot( vec4& a, vec4& b );

//void	cross( vec3& v, const vec3& a, const vec3& b );
//void	vnormalize( vec3& a );
