////////
!vs
////////
#version 330

uniform mat4 matProj;
uniform mat4 matModel;
uniform mat4 matView;
uniform vec3	ColModel;

in	vec4 Position;
in	vec3 Normal;
out	vec3 nml;
out	vec4 pos;

//-----------------------------------------------------------------------------
void main()
//-----------------------------------------------------------------------------
{
	vec3	V = vec3(Position*matModel);
	vec3	P = vec3(0,0,0);
	vec3	N = vec3(0,0,-1);
	vec3	E = normalize( V-P );
	float	d = 4;
	float	t = -d/dot(E,N);
	vec3	Q = E*t + P;

//	Q.z = 1.0;

//Q.x /= 20; 
//Q.y /= 20; 
#if 1
	float	z = V.z+4;

	Q.x = V.x/ z;
	Q.y = V.y/z;
	Q.z = 1.00;

#endif

	gl_Position = vec4(Q,1);
	

	nml = Normal;
}

////////
!fs
////////
#version 330

uniform vec3	ColModel;

in	vec3	nml;
in	vec4	pos;
out	vec4	Color0;


float	a[] = float[](1,2,3,4);

//-----------------------------------------------------------------------------
void main ()
//-----------------------------------------------------------------------------
{
	vec3	col;

	vec3	L = normalize( vec3(1,-1.5,2) );

	float	d = dot( -L, normalize(nml) ); 

	if ( pos.z < 0 )
	{
		col = -pos.zzz;
		col = vec3( 0,1,0 )*d;
	}

//	col = vec3( 1,-d,0 );
//	col = vec3(d);

	col = ColModel * d;

	Color0 = vec4( col, 1.0);


}

