uniform sampler2D Texture;
varying	vec2 uv;

//-----------------------------------------------------------------------------
void main (void)
//-----------------------------------------------------------------------------
{

//	gl_FragColor = texture2D(Texture, uv.xy);
	gl_FragColor = vec4(1,1,1,1);


}

