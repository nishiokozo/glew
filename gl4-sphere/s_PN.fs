#version 330

in	vec3	nml;
in	vec4	pos;
out	vec4	Color0;

float	a[] = float[](1,2,3,4);

//-----------------------------------------------------------------------------
void main ()
//-----------------------------------------------------------------------------
{
	vec3	col = vec3( 1,1,0 );

	vec3	L = vec3(0,0,1);

	float	d = dot( -L, normalize(nml) ); 

	if ( pos.z < 0 )
	{
		col = -pos.zzz;
		col = vec3( 0,1,0 )*d;
	}

	col = vec3( 0,d,0 );

	Color0 = vec4( col, 1.0);


}

