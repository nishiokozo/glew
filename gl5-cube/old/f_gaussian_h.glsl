!vs
//uniform mat4 Projection;
//uniform mat4 Model;
//uniform mat4 View;
//
attribute vec4 Position;
//attribute vec2 Uv;

varying	vec2 uv;

//-----------------------------------------------------------------------------
void main(void)
//-----------------------------------------------------------------------------
{
//	gl_Position = Projection * View *Model * Position;
//	gl_Position = Projection * Position;
//
//	uv = Uv;
	gl_Position = Position;
	uv = Position.xy*0.5+0.5;
}


!fs
#version 120
uniform sampler2D	Tex0;

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

	col  = texture2D( Tex0, uv ) * Weight[0] ;
	
	int	i;
	for ( i = 1 ; i < Length ; i++ )
	{
		float	ofs = float(i)*ofset;
		col  += texture2D(Tex0, uv + vec2( ofs, 0 ) ) * Weight[i];
		col  += texture2D(Tex0, uv + vec2(-ofs, 0 ) ) * Weight[i];
	}
	col.a = 1.0;


#else
	col  = texture2D( Tex0, uv ) * 0.2 ;
	
	int	i;
	for ( i = 1 ; i < Length ; i++ )
	{
		float	ofs = float(i)/512.0;
		col  += texture2D(Tex0, uv + vec2(  0,  ofs ) ) * 0.05;
		col  += texture2D(Tex0, uv - vec2(  0,  ofs ) ) * 0.05;
	}
#endif

	gl_FragColor = col;


}