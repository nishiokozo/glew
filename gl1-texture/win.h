
class	Win
{
	public:
	boolean	m_bError;
	HWND	m_hWnd;
	HDC		m_hDC;

	Win( int width, int height );
	~Win();
	boolean update();
};
