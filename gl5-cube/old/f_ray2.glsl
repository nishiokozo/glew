///////////////////////////////////////////////////////////////////////////////
!vs
#version 330

in	vec4 Position;
out vec3 eye;
out vec3 vec;
out vec3 scr;

void main()
{
	float	aspect = 960.0/540.0;

	gl_Position = Position;

	scr = Position.xyz;

	scr.x *= aspect;


}

///////////////////////////////////////////////////////////////////////////////
!fs
#version 330


in vec3 scr;
out vec4 color;


void main()
{
	vec3 col = vec3(0);
/*

	vec3 E = vec3( 0, 0, 1 );
	vec3 P = vec3( scr.x, scr.y, 0 );

	vec3 Q = vec3( 1, 0.5, -0.3 );
	float r = 0.5;

	float	x = ( P.x - Q.x );
	float	y = ( P.y - Q.y );
	float	z = ( P.z - Q.z );
	float	l = r*r - length( P-Q );

	if ( l < r ) 
	{
		col = vec3(1,0,1);
	}
*/
		

	float	r = 0.5;
	float	x = scr.x;
	float	y = scr.y;
	float 	z = sqrt( r*r - x*x - y*y );
	float 	l = r*r - x*x - y*y;
	
	if ( l > 0 )
	{
		float t = 4-sqrt(l);
		col = vec3(t,1,1);
	}
	else
	{
		col = vec3(0,0,1);
	}
	
//		col = vec3(1,1,1)*0.20;


	color = vec4( col, 1.0);

}
