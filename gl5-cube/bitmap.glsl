///////////////////////////////////////////////////////////////////////////////
!vs
#version 330

in	vec4 Position;
in	vec2 Uv;

out	vec2 uv;

void main()
{
	gl_Position = Position;

	uv = Uv;
//	uv = Position.xy*0.5+0.5;

}

///////////////////////////////////////////////////////////////////////////////
!fs
#version 330
uniform sampler2D image0;
in	vec2 uv;
out vec4 color;


void main()
{
	color = texture2D( image0, uv );
}
