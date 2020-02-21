#version 330

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

in	vec4 Position;
in	vec3 Normal;
out	vec3 nml;
out	vec4 pos;


//-----------------------------------------------------------------------------
void main()
//-----------------------------------------------------------------------------
{
	vec3	eye = vec3(0,0,0);
	pos = Position * M;

	vec3	V = pos.xyz - eye;

	float	l = sqrt( dot( V, V ) );


	float	f = 2;

	pos.x /= f;
	pos.y /= f;
	pos.z = 1.00;


	gl_Position = pos;

	
	

	nml = Normal;
}
