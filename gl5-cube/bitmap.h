////////////////////////////////////////////////////////////////////////////////
class Bitmap
////////////////////////////////////////////////////////////////////////////////
{
public:

	int		m_type;
	GLuint	m_hdl_idTexture;
	float*	m_pBuf;
	int		m_width;
	int		m_height;
	
	void	PutColor( int x, int y, float r, float g, float b );

	~Bitmap();
	Bitmap( int width, int height, int type, bool nearest_linear );
	void 	UpdateTexture();
	GLuint	GetTexture();
};
