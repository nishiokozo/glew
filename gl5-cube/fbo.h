////////////////////////////////////////////////////////////////////////////////
class	Fbo
////////////////////////////////////////////////////////////////////////////////
{
public:
	GLuint	m_hdlFbo;
	GLuint	m_color_tex;
	GLuint	m_depth_tex;

	int		m_w;
	int		m_h;

	Fbo( int w, int h, int type, bool nearest_linear );
};