#version 120
uniform sampler2D	Texture;

uniform int		Length;

varying	vec2	uv;


//uniform float	Weight[] = float[](0.133176,0.125979,0.106639,0.080775,0.054750,0.033208,0.018023,0.008753,0.003804,0.001479);
uniform float	Weight[100];
uniform float	ofset;
//uniform float	Weight[10] = float[](0.133176,0.125979,0.106639,0.080775,0.054750,0.033208,0.018023,0.008753,0.003804,0.001479);

//-----------------------------------------------------------------------------
void main (void)
//-----------------------------------------------------------------------------
{
	vec4	col = vec4( 0.0, 0.0, 0.0, 1.0 );

#if 1

	col  = texture2D( Texture, uv ) * Weight[0] ;
	
	int	i;
	for ( i = 1 ; i < Length ; i++ )
	{
		float	ofs = float(i)*ofset;
//		float	ofs = float(i)*(1.0/512.0);
		col  += texture2D(Texture, uv + vec2(  0,  ofs ) ) * Weight[i];
		col  += texture2D(Texture, uv + vec2(  0, -ofs ) ) * Weight[i];
	}
	col.a = 1.0;


#else
	col  = texture2D( Texture, uv ) * 0.2 ;
	
	int	i;
	for ( i = 1 ; i < Length ; i++ )
	{
		float	ofs = float(i)/512.0;
		col  += texture2D(Texture, uv + vec2(  0,  ofs ) ) * 0.05;
		col  += texture2D(Texture, uv - vec2(  0,  ofs ) ) * 0.05;
	}
#endif

	gl_FragColor = col;


}