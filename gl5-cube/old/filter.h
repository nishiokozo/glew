////////////////////////////////////////////////////////////////////////////////
class	Filter
////////////////////////////////////////////////////////////////////////////////
{
public:

	//	for common
	int		m_cntIndex;
	GLuint	m_hdlVertex_Position;
	GLuint	m_hdlVertex_Index;
	GLuint	m_hdlProgram;
	int		m_hdl_Position;
	int		m_hdl_Tex0;
	int		m_hdl_Tex1;

	//	for	gaussian
	int		m_hdl_Length;
	int		m_hdl_Weight;
	int		m_hdl_Offset;
	
	~Filter();
	Filter( const char* fnShader );
	void DrawTexture( int tex);
	void DrawFilter();
	void DrawGaussian( int tex, float pitch, int lenWeight, float* tblWeight );
	void Draw_blend( int tex0, int tex1 );
	void ReloadShader( const char* fnShader );

};
