uniform mat4 Projection;
uniform mat4 Model;
uniform mat4 View;

attribute vec4 Position;
attribute vec2 Uv;

varying	vec2 uv;

//-----------------------------------------------------------------------------
void main(void)
//-----------------------------------------------------------------------------
{
//	gl_Position = Projection * View *Model * Position;
	gl_Position = Projection * Position;

	uv = Uv;
}
