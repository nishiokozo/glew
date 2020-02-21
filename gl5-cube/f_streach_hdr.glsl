!vs
attribute vec4 Position;
varying	vec2 uv;

//-----------------------------------------------------------------------------
void main(void)
//-----------------------------------------------------------------------------
{
	gl_Position = Position;
	uv = Position.xy*0.5+0.5;

}

!fs
uniform sampler2D Tex0;
varying	vec2 uv;

//-----------------------------------------------------------------------------
void main (void)
//-----------------------------------------------------------------------------
{
	
	vec4	col = texture2D(Tex0, uv.xy);


	col = max( vec4(0.0), col - vec4(1,1,1,0) );

	gl_FragColor = col;


}

