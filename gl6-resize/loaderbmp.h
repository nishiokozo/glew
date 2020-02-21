
class LoaderBmp
{
	bool	m_flgActive;

	int 	m_format;
	int  	m_width;
	int  	m_height;
	void*	m_pBuf;

public:

	LoaderBmp( const char *fname );
	~LoaderBmp();
	int		isActive() { return m_flgActive; };

	int		format() { return m_format; };
	int		width() { return m_width; };
	int		height() { return m_height; };
	void*	address() { return m_pBuf; };

};

