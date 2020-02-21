class	Shader
{
public:


	GLuint	m_hdlProgram;
	int		m_hdl_buf_Position;
	int		m_hdl_Uv;
	int		m_hdl_var_Projection;
	int		m_hdl_var_Model;
	int		m_hdl_bufer_Normal;
	GLuint	m_hdl_idTexture;
	int	m_hdl_var_ColModel;

	Shader( const char* fn );
//	Shader();
	void DrawPN( const Shape& shape, const mat4& M, const mat4& P );



};
