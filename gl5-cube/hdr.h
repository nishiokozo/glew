////////////////////////////////////////////////////////////////////////////////
class	Hdr
////////////////////////////////////////////////////////////////////////////////
{

	Filter*	m_pStreach_hdr;
	Filter*	m_pStreach_blend;
	Filter*	m_pGaussianv;
	Filter*	m_pGaussianh;
	Fbo*	m_pFbo1;
	Fbo*	m_pFbo2;


public:
	Hdr( int width, int height, int type, bool nearest_linear );
	void	DrawBloom( GLuint hdlTex, int lenWeight, double s );
	void	DrawDirect( GLuint hdlTex );


};
