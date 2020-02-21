#include "sys.h"

Sys	sys( "sys.ini" );

#define	PUT_SPF(s) \
		{\
			double f = m_frequency.QuadPart;\
			static	LARGE_INTEGER tm0,tm1;\
			LARGE_INTEGER m_frequency; \
			QueryPerformanceFrequency(&m_frequency);\
			QueryPerformanceCounter(&tm1);\
			printf("%s %f\n", s, (double)(tm1.QuadPart-tm0.QuadPart) / (double)m_frequency.QuadPart);\
			QueryPerformanceCounter(&tm0);\
		}


static	struct Key
{
	BYTE	prev[256];
	BYTE	state[256];
	bool	flgTrigger[256];
	bool	flgRelease[256];
	bool	flgToggle[256];
} g_key;

//-----------------------------------------------------------------------------
void key_init()
//-----------------------------------------------------------------------------
{
	for ( int i ; i < 256 ; i++ )
	{
		g_key.prev[i] = 0;
		g_key.state[i] = 0;
		g_key.flgTrigger[i] = 0;
		g_key.flgRelease[i] = 0;
		g_key.flgToggle[i] = 0;
	}
	GetKeyboardState( g_key.state );
}

//-----------------------------------------------------------------------------
void	key_update()
//-----------------------------------------------------------------------------
{

	GetKeyboardState( g_key.state );

	for ( int i=0 ; i < 256 ; i++ )
	{

		g_key.flgTrigger[i] = false;
		g_key.flgRelease[i] = false;

		if( (g_key.state[i] & 0x80) && !(g_key.prev[i] &0x80) )
		{
			g_key.flgTrigger[i] = true;
			g_key.flgToggle[i] = !g_key.flgToggle[i];
		}
		if( !(g_key.state[i] & 0x80) && (g_key.prev[i] &0x80) )
		{
			g_key.flgRelease[i] = true;
		}

		g_key.prev[i] = g_key.state[i];
	}
}
//------------------------------------------------------------------------------
static	LRESULT CALLBACK WndProc
//------------------------------------------------------------------------------
(
	  HWND	hWnd
	, UINT	uMsg
	, WPARAM wParam
	, LPARAM lParam
);



//-----------------------------------------------------------------------------
bool	Sys::key_Press( int val )
//-----------------------------------------------------------------------------
{
//	if ( GetKeyState( val ) &0x80 ) return true;
/*
	bool	ret = false;
	BYTE state[256];
	GetKeyboardState( state );

	if( state[val] & 0x80 )
	{
		ret = true;
	}
	return ret;
*/
	return ( (g_key.state[val] & 0x80) != 0 );
}
//-----------------------------------------------------------------------------
bool	Sys::key_Toggle( int val )
//-----------------------------------------------------------------------------
{
/*
	bool	ret = false;
	static	BYTE prev[256];
	static	BYTE flg[256];
	BYTE state[256];
	GetKeyboardState( state );

	if( (state[val] & 0x80) && !(prev[val] &0x80) )
	{
		flg[val] = !flg[val];
	}

	prev[val] = state[val];
	return flg[val];
*/

	return g_key.flgToggle[val];
}

//-----------------------------------------------------------------------------
bool	Sys::key_Trigger( int val )
//-----------------------------------------------------------------------------
{
/*	bool	ret = false;
	static	BYTE prev[256];
	BYTE state[256];
	GetKeyboardState( state );

	bool	flg = false;

	if( (state[val] & 0x80) && !(prev[val] &0x80) )
	{
		flg = true;
	}

	prev[val] = state[val];
	return flg;
*/
	return g_key.flgTrigger[val];
}


//------------------------------------------------------------------------------
void Sys::EnableOpenGL(HWND hWnd, HDC * pHDC, HGLRC * hGLRC)
//------------------------------------------------------------------------------
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	// get the device context (DC)
	(*pHDC) = GetDC( hWnd );
	
	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( (*pHDC), &pfd );
	SetPixelFormat( (*pHDC), format, &pfd );
	
	// create and enable the render context (RC)
	*hGLRC = wglCreateContext( (*pHDC) );
	wglMakeCurrent( (*pHDC), *hGLRC );

	// GLEW初期化
	{
		int err = glewInit();
		if (err != GLEW_OK)
		{
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
	}

	wglSwapIntervalEXT(0);

//	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
//	glClear( GL_COLOR_BUFFER_BIT );

	//	version
	printf( "GL_VERSION                 :%s\n", glGetString(GL_VERSION) );
	printf( "GL_SHADING_LANGUAGE_VERSION:%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );

	// Clear
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 1
	// Culling
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace( GL_CW );

	// Depth
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);	

	// Alpha
	glDisable(GL_ALPHA_TEST);
#endif


}

//------------------------------------------------------------------------------
void Sys::DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hGLRC)
//------------------------------------------------------------------------------
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hGLRC );
	ReleaseDC( hWnd, hDC );

}

//-----------------------------------------------------------------------------
void	Sys::gl_LoadTexture( int target, GLuint hdl_Texture, int innerformat, int width, int height, int format, int type, void* pBuf )
//-----------------------------------------------------------------------------
{
	glBindTexture( target, hdl_Texture );
	glTexImage2D( target, 0, innerformat, width, height, 0, format, type, pBuf );
	glBindTexture( target, 0 );
}

//-----------------------------------------------------------------------------
void	Sys::gl_CreateTexture( GLuint*  p_hdl_Texture, int target, int format, int width, int height, int min, int mag, int wrap )
//-----------------------------------------------------------------------------
{

	glGenTextures( 1, p_hdl_Texture );
	glBindTexture( target, *p_hdl_Texture );
	glTexParameterf( target, GL_TEXTURE_MIN_FILTER, min );
	glTexParameterf( target, GL_TEXTURE_MAG_FILTER, mag );
	glTexParameterf( target, GL_TEXTURE_WRAP_S, wrap );
	glTexParameterf( target, GL_TEXTURE_WRAP_T, wrap );
	glTexImage2D( target, 0, format, width, height, 0, GL_RGBA, GL_FLOAT, 0 );
	glBindTexture( target, 0 );
}
//-----------------------------------------------------------------------------
void	Sys::gl_SetTexture( int target, int hdl_Texture, int num, GLuint hdl_idTexture )
//-----------------------------------------------------------------------------
{
	glUniform1i( hdl_Texture, num );
	glActiveTexture( GL_TEXTURE0 + num );
	glBindTexture( target, hdl_idTexture );
}
//-----------------------------------------------------------------------------
void	Sys::gl_SetBuffer( GLint hdlTag, GLuint hdlBuffer, int type, int sizeVal, int defVal )
//-----------------------------------------------------------------------------
{
	glEnableVertexAttribArray( hdlTag );
	glBindBuffer( type, hdlBuffer  );
	glVertexAttribPointer( hdlTag, sizeVal, defVal, GL_FALSE, 0, 0);
}
//-----------------------------------------------------------------------------
void	Sys::gl_CreateBuffer( GLuint* pHdl, int type, int size, void* tbl )
//-----------------------------------------------------------------------------
{
	glGenBuffers(1, &(*pHdl) );
	glBindBuffer(type, (*pHdl) );
	glBufferData(type, size, tbl, GL_STATIC_DRAW);

}

//-----------------------------------------------------------------------------
bool	Sys::file_malloc_Load( const char* fname, char** ppBuf, int* pLen )
//-----------------------------------------------------------------------------
{
	char*	fbuf = 0;
	int		flen = 0;
	{

		FILE*	fp = 0;
		if ((fp = fopen(fname, "rb")) == NULL)
		{
			printf( "File error [%s]\n", fname );
			return	false;
		}
		else
		{
			{
				fseek(fp, 0, SEEK_END);
				flen = (int)ftell(fp);
				fseek(fp, 0, SEEK_SET);
			}
			{
				fbuf = (char*)malloc( flen );
//				fbuf = new char( flen );
				fread(fbuf, 1, flen, fp);
			}
			fclose( fp );
		}
	}
	(*ppBuf) = fbuf;
	(*pLen) = flen;

	return	true;
}





//------------------------------------------------------------------------------
Sys::~Sys()
//------------------------------------------------------------------------------
{
	if ( m_env_pBuf ) {free( m_env_pBuf );m_env_pBuf = 0;}
//	mem_free( &m_env_pBuf );

	DisableOpenGL( m_hWnd, m_hDC, m_hGLRC );
	DestroyWindow( m_hWnd );

}


//------------------------------------------------------------------------------
Sys::Sys( const char* fn )
//------------------------------------------------------------------------------
{
	memset( this, 0, sizeof(this) );

	QueryPerformanceFrequency(&m_frequency);

	key_init();


	if ( fn )
	{
		file_malloc_Load( fn, &m_env_pBuf, &m_env_len );
	}
	else
	{
		const char* str = "name none\nwidth 640\nheight 480\n";
		m_env_len = strlen( str );
		m_env_pBuf = (char*)malloc( m_env_len );
		strncpy( m_env_pBuf, str, m_env_len );
	}

		m_bError = 0;
		m_hWnd = 0;
		m_hDC = 0;
		m_hGLRC = 0;


//		m_pProc = 0;



	m_w = env_GetInt("width"); 
	m_h = env_GetInt("height");

	HINSTANCE	hInstance;
	hInstance	 = GetModuleHandle( NULL );

	WNDCLASSEX wc;
	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.style			= 0;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;  
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground 	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  	= MAKEINTRESOURCE( NULL );
	wc.lpszClassName 	= "yukizokin.sys";
	wc.hIconSm			= NULL;

	if ( 0 == RegisterClassEx( &wc ) ) 
	{
		m_bError = true;	
	}
	else
	{

		{
			RECT rc;
			SetRect(&rc, 0, 0, m_w, m_h );
			AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, 0);

			m_hWnd = CreateWindowEx (
				// message:
				// 1)WM_GETMINMAXINFO
				// 2)WM_NCCREATE
				// 3)WM_NCCALCSIZE
				// 4)WM_CREATE
				  0
				, wc.lpszClassName
				, (env_GetString("name") + ":" + string(__DATE__) ).c_str()
				, WS_OVERLAPPEDWINDOW   	// window style
				, 250+rc.left   	        // horizontal position of window
				, 250+rc.top				// vertical position of window
				, rc.right-rc.left 			// window width
				, rc.bottom-rc.top			// window height
				, NULL                    // handle to parent or owner window
				, NULL                    // handle to menu, or child-window identifier
				, hInstance               // handle to application instance
				, 0				       // pointer to window-creation data
			);
		}

		EnableOpenGL( m_hWnd, &m_hDC, &m_hGLRC );

		ShowWindow ( m_hWnd, SW_NORMAL );


	}
};

//-----------------------------------------------------------------------------
bool	Sys::env_FindValue( const char* name, char* str, int len )
//-----------------------------------------------------------------------------
{
	int		n = 0;
	int		cnt = 0;
	bool	flgFind = false;
	
	str[0] = 0;

	for ( int i = 0 ; i < len  ; i++  )
	{
		char	c = m_env_pBuf[i];

		bool	flgCtrl = ( iscntrl(c) || isspace(c) ) ;
		bool	flgEow = ( flgCtrl == true || i+1 == len ) ;
		bool	flgEol = ( c == 0x0a || i+1 == len ) ;

		if ( flgCtrl == false ) str[n++] = c;
			
		if ( flgEow )
		{
			str[n] = 0;
			n = 0;

			if ( cnt == 0 && strcmp( str, name ) == 0 ) 
			{
				flgFind = true;
			}

			if ( flgFind && cnt > 0 ) break;
			
			cnt++;
		}
		
		if ( flgEol )
		{
			cnt = 0;
		}
		
	}
	
	if ( flgFind == false ) str[0] = 0;

	return flgFind;
}

//-----------------------------------------------------------------------------
float	Sys::env_GetFloat( const char* name )
//-----------------------------------------------------------------------------
{
	if ( env_FindValue( name, m_env_str, m_env_len ) ) return atof( m_env_str );
	return 0.0f;
}
//-----------------------------------------------------------------------------
int		Sys::env_GetInt( const char* name )
//-----------------------------------------------------------------------------
{
	if ( env_FindValue( name, m_env_str, m_env_len ) ) return atoi( m_env_str );
	return 0;
}
//-----------------------------------------------------------------------------
string	Sys::env_GetString( const char* name )
//-----------------------------------------------------------------------------
{
	env_FindValue( name, m_env_str, m_env_len );
	return m_env_str;
}
/*
//------------------------------------------------------------------------------
void	Sys::SysSetProc( IProc& fw )
//------------------------------------------------------------------------------
{
	m_pProc = &fw;
}
*/
//------------------------------------------------------------------------------
void	 Sys::Sync()
//------------------------------------------------------------------------------
{
				if (0)
				{
					SwapBuffers( m_hDC );
				}
				else
				{
					// 描画速度計測
					static	LARGE_INTEGER f0;
					static	LARGE_INTEGER f1;
					static	double timerPaint = 0;
					static	double counterPaint = 0;

					QueryPerformanceCounter(&f0);

					//glFenceSync — create a new sync object and insert it into the GL command stream
					GLsync  SyncObject = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );

					//glWaitSync — instruct the GL server to block until the specified sync object becomes signaled
					glWaitSync( SyncObject, 0, GL_TIMEOUT_IGNORED );

					SwapBuffers( m_hDC );

					//glClientWaitSync — block and wait for a sync object to become signaled
					glClientWaitSync( SyncObject, 0, GL_TIMEOUT_IGNORED );

					glDeleteSync( SyncObject );

					QueryPerformanceCounter(&f1);


					// 描画速度計測
					timerPaint += (double)(f1.QuadPart-f0.QuadPart) / (double)m_frequency.QuadPart;
					counterPaint++;

					m_timerSec += m_timerFrame;
					if ( m_timerSec > 1.0 )
					{
		//				PUT_SPF( "timer sec=" );
//						printf("paint %fms\n", 1000 * timerPaint / counterPaint );
						m_measure_timePerFrame_ms = 1000 * timerPaint / counterPaint;
						m_measure_flgInfomation = true;
//						printf("paint-cnt %f %f\n", timerPaint, counterPaint );
						m_timerSec = 0;
						timerPaint = 0;
						counterPaint = 0;
					}
				}

}
//------------------------------------------------------------------------------
bool	Sys::measure_GetTimerPaint()
//------------------------------------------------------------------------------
{
	bool ret = m_measure_flgInfomation;
	m_measure_flgInfomation = false;

	return ret;
}
//------------------------------------------------------------------------------
bool	 Sys::Update( double timeSync )
//------------------------------------------------------------------------------
{
	bool	ret = true;

	Sync();
	//		PUT_SPF( "timer frame=" );
	m_timerFrame = 0;  
	
	while (true)
	{
		MSG	msg;
		if (PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE) )
		{
			if (msg.message == WM_QUIT)
			{
				ret = false;
				break;
			}
			else
			{
				DispatchMessage(&msg);
			}
		}
		else
		{
			QueryPerformanceFrequency(&m_frequency);

			static	LARGE_INTEGER tm0;
			static	LARGE_INTEGER tm1;

			QueryPerformanceCounter(&tm1);
			m_timerFrame += (double)(tm1.QuadPart-tm0.QuadPart) / (double)m_frequency.QuadPart;
			QueryPerformanceCounter(&tm0);

			if ( m_timerFrame >= timeSync )
			{
				break;
			}

		}
	}

	return	ret;
}
/*
//------------------------------------------------------------------------------
void	 Sys::SysRun()
//------------------------------------------------------------------------------
{
	while ( Update(1.0/60.0) )
	{
			if ( m_pProc ) m_pProc->Paint();
	}

}

*/


//------------------------------------------------------------------------------
double	Sys::GetSecond()
//------------------------------------------------------------------------------
{
	double f = m_frequency.QuadPart;
	LARGE_INTEGER tm;
	QueryPerformanceCounter(&tm);

	return (double)(tm.QuadPart) / f;
}

#define MAX_SHADER_LOG_SIZE		(1024)
static	char s_logBuffer[MAX_SHADER_LOG_SIZE]; 
//-----------------------------------------------------------------------------
bool Sys::printShaderInfoLog(
//-----------------------------------------------------------------------------
	GLuint shader
)
{
	int logSize;
	int length;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &logSize);

	if (logSize > 1)
	{
		glGetShaderInfoLog(shader, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);


		printf( "%s", s_logBuffer );
		if ( s_logBuffer[0] =='0' ) 
		{
			return false;
		}

	}
	return true;
}
//-----------------------------------------------------------------------------
void Sys::shader_Remove( GLuint hdlProgram )
//-----------------------------------------------------------------------------
{
	glDeleteProgram( hdlProgram );
}
//-----------------------------------------------------------------------------
void Sys::shader_LoadFile( GLuint*	pHdlProgram, const char* fn )
//-----------------------------------------------------------------------------
{

	GLuint hdlProgram = glCreateProgram();

	char*	pBuf = 0; 
	int		len = 0;

	file_malloc_Load( fn, &pBuf, &len );

	GLint result;

	struct SHADER
	{
		enum
		{
			NONE,
			VS,
			FS,

			MAX
		};
		int		ofs;
		char*	pBuf;
		int		len;
		int		type;
	} inf[ SHADER::MAX ] =
	{
		{0,0,0,0},
		{0,0,0,GL_VERTEX_SHADER},
		{0,0,0,GL_FRAGMENT_SHADER},
	};
	{
		int	shd = SHADER::NONE; 
		int	cntLF = 0;
		for ( int i = 0 ; i  < len ; i++ )
		{
			if ( pBuf[i] == 0x0a ) cntLF++;
			if ( '!' ) 
			{
				int	flg = false;
				if ( strncmp( &pBuf[i], "!vs",3 ) == 0 ) 
				{
					shd = SHADER::VS;
					flg = true;
				}
				if ( strncmp( &pBuf[i], "!fs",3 ) == 0 )
				{
					shd = SHADER::FS;
					flg = true;
				}
				if ( flg )
				{
					pBuf[i+0] = ' ';
					pBuf[i+1] = ' ';
					pBuf[i+2] = ' ';
					inf[shd].pBuf = &pBuf[i];
					inf[shd].len = cntLF;
					inf[shd].ofs = cntLF;
				}

			}
			inf[shd].len++;
		}
	}

	bool flgOK = true;
	for ( int shd = 1 ; shd  < SHADER::MAX ; shd ++ )
	{
		GLuint hdlShader = glCreateShader( inf[shd].type );

		{
			// エラー表示の行番号をそろえるためのオフセット調整
			int	s = 0;
			char*	str = new char[ inf[shd].len+1  ];
			for ( int i = 0 ; i < inf[shd].len ; i++ )
			{
				if ( i < inf[shd].ofs ) str[i] = 0x0a;
				else
				{
					str[i] = inf[shd].pBuf[s];
					s++;
				}
			}	
			str[inf[shd].len] = 0;
//			printf("[[[[%s]]]]]\n", str );	
			glShaderSource( hdlShader, 1, (const char **)&str, &inf[shd].len);
			delete str;
		}

		glCompileShader( hdlShader );
		glGetShaderiv( hdlShader, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			printShaderInfoLog( hdlShader );
			if ( shd == SHADER::VS ) fprintf(stderr, "Compile error at Vertex Shader in [%s]\n", fn);
			if ( shd == SHADER::FS ) fprintf(stderr, "Compile error at Fragment Shader in [%s]\n", fn);
			flgOK = false;
			glDeleteShader( hdlShader );
			break;
		}
		if ( false == printShaderInfoLog( hdlShader ) )
		{
			if ( shd == SHADER::VS ) fprintf(stderr, "Compile warning at Vertex Shader in [%s]\n", fn);
			if ( shd == SHADER::FS ) fprintf(stderr, "Compile warning at Fragment Shader in [%s]\n", fn);
		}

		glAttachShader( hdlProgram, hdlShader );
		glDeleteShader( hdlShader );
	}
//	mem_free( &pBuf );
	if ( pBuf ) {free( pBuf );pBuf = 0;}


	if ( (*pHdlProgram) != 0 ) 
	{
		glDeleteProgram( (*pHdlProgram) );
		(*pHdlProgram) = 0;
	}

	if ( flgOK )
	{
		glLinkProgram( hdlProgram );
		glValidateProgram( hdlProgram );

		(*pHdlProgram) = hdlProgram;
	}
	else
	{
		glDeleteProgram( hdlProgram );
	}
	
}

//-----------------------------------------------------------------------------
bool Sys::shader_Compile(
//-----------------------------------------------------------------------------
	int		hdlProgram,
	int		type,
	const char*	fn
)
{
	GLint result;

	char*	pBuf = 0; 
	int		len = 0;

	file_malloc_Load( fn, &pBuf, &len );
	GLuint hdl = glCreateShader(type);

	glShaderSource( hdl, 1, (const char **)&pBuf, &len);
	glCompileShader( hdl );
	glGetShaderiv( hdl, GL_COMPILE_STATUS, &result);

	printShaderInfoLog( hdl );
	if (result == GL_FALSE)
	{
		fprintf(stderr, "Compile error at [%s]\n", fn);
	}

	glAttachShader( hdlProgram, hdl );
	glDeleteShader( hdl );
//	mem_free( &pBuf );
	if ( pBuf ) {free( pBuf );pBuf = 0;}
	return	true;
}
//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc
//------------------------------------------------------------------------------
(
	  HWND	hWnd
	, UINT	uMsg
	, WPARAM wParam
	, LPARAM lParam
)
{
	switch( uMsg ) 
	{
	case WM_CREATE:				/*printf( "WM_CREATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );*/break;
	case WM_SHOWWINDOW:			/*printf( "WM_SHOWWINDOW(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );*/	break;
	case WM_CLOSE:	printf( "WM_CLOSE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;

	case WM_APP:		printf( "WM_APP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_AFXFIRST:	printf( "WM_AFXFIRST(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_AFXLAST:	printf( "WM_AFXLAST(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_ASKCBFORMATNAME:	printf( "WM_ASKCBFORMATNAME(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CANCELJOURNAL:	printf( "WM_CANCELJOURNAL(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CANCELMODE:	printf( "WM_CANCELMODE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CHANGECBCHAIN:	printf( "WM_CHANGECBCHAIN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CHARTOITEM:	printf( "WM_CHARTOITEM(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CHILDACTIVATE:	printf( "WM_CHILDACTIVATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CLEAR:	printf( "WM_CLEAR(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_COMMAND:	printf( "WM_COMMAND(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_COMMNOTIFY:	printf( "WM_COMMNOTIFY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_COMPACTING:	printf( "WM_COMPACTING(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_COMPAREITEM:	printf( "WM_COMPAREITEM(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_COPY:	printf( "WM_COPY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_COPYDATA:	printf( "WM_COPYDATA(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CTLCOLORBTN:	printf( "WM_CTLCOLORBTN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CTLCOLORDLG:	printf( "WM_CTLCOLORDLG(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CTLCOLOREDIT:	printf( "WM_CTLCOLOREDIT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CTLCOLORLISTBOX:	printf( "WM_CTLCOLORLISTBOX(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CTLCOLORMSGBOX:	printf( "WM_CTLCOLORMSGBOX(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CTLCOLORSCROLLBAR:	printf( "WM_CTLCOLORSCROLLBAR(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CTLCOLORSTATIC:	printf( "WM_CTLCOLORSTATIC(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_CUT:	printf( "WM_CUT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_DEADCHAR:	printf( "WM_DEADCHAR(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_DELETEITEM:	printf( "WM_DELETEITEM(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_DESTROYCLIPBOARD:	printf( "WM_DESTROYCLIPBOARD(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_DEVICECHANGE:	printf( "WM_DEVICECHANGE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_DEVMODECHANGE:	printf( "WM_DEVMODECHANGE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_DISPLAYCHANGE:	printf( "WM_DISPLAYCHANGE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_DRAWCLIPBOARD:	printf( "WM_DRAWCLIPBOARD(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_DRAWITEM:	printf( "WM_DRAWITEM(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_DROPFILES:	printf( "WM_DROPFILES(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_ENABLE:	printf( "WM_ENABLE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_ENDSESSION:	printf( "WM_ENDSESSION(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_FONTCHANGE:	printf( "WM_FONTCHANGE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_GETDLGCODE:	printf( "WM_GETDLGCODE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_GETFONT:	printf( "WM_GETFONT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_GETHOTKEY:	printf( "WM_GETHOTKEY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_GETTEXTLENGTH:	printf( "WM_GETTEXTLENGTH(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_HANDHELDFIRST:	printf( "WM_HANDHELDFIRST(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_HANDHELDLAST:	printf( "WM_HANDHELDLAST(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_HELP:	printf( "WM_HELP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_HOTKEY:	printf( "WM_HOTKEY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_HSCROLL:	printf( "WM_HSCROLL(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_HSCROLLCLIPBOARD:	printf( "WM_HSCROLLCLIPBOARD(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_ICONERASEBKGND:	printf( "WM_ICONERASEBKGND(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_INITDIALOG:	printf( "WM_INITDIALOG(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_INITMENUPOPUP:	printf( "WM_INITMENUPOPUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_INPUTLANGCHANGE:	printf( "WM_INPUTLANGCHANGE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_INPUTLANGCHANGEREQUEST:	printf( "WM_INPUTLANGCHANGEREQUEST(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDICASCADE:	printf( "WM_MDICASCADE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDICREATE:	printf( "WM_MDICREATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDIDESTROY:	printf( "WM_MDIDESTROY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDIGETACTIVE:	printf( "WM_MDIGETACTIVE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDIICONARRANGE:	printf( "WM_MDIICONARRANGE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDIMAXIMIZE:	printf( "WM_MDIMAXIMIZE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDINEXT:	printf( "WM_MDINEXT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDIREFRESHMENU:	printf( "WM_MDIREFRESHMENU(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDIRESTORE:	printf( "WM_MDIRESTORE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDISETMENU:	printf( "WM_MDISETMENU(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MDITILE:	printf( "WM_MDITILE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_MEASUREITEM:	printf( "WM_MEASUREITEM(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NEXTMENU:	printf( "WM_NEXTMENU(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NCLBUTTONUP:	printf( "WM_NCLBUTTONUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NCMBUTTONDBLCLK:	printf( "WM_NCMBUTTONDBLCLK(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NCMBUTTONDOWN:	printf( "WM_NCMBUTTONDOWN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NCMBUTTONUP:	printf( "WM_NCMBUTTONUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NCRBUTTONDBLCLK:	printf( "WM_NCRBUTTONDBLCLK(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NCRBUTTONDOWN:	printf( "WM_NCRBUTTONDOWN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NCRBUTTONUP:	printf( "WM_NCRBUTTONUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NEXTDLGCTL:	printf( "WM_NEXTDLGCTL(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NOTIFY:	printf( "WM_NOTIFY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NOTIFYFORMAT:	printf( "WM_NOTIFYFORMAT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_NULL:	printf( "WM_NULL(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_PAINTCLIPBOARD:	printf( "WM_PAINTCLIPBOARD(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_PAINTICON:	printf( "WM_PAINTICON(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_PALETTECHANGED:	printf( "WM_PALETTECHANGED(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_PALETTEISCHANGING:	printf( "WM_PALETTEISCHANGING(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_PARENTNOTIFY:	printf( "WM_PARENTNOTIFY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_PASTE:	printf( "WM_PASTE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_PENWINFIRST:	printf( "WM_PENWINFIRST(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_PENWINLAST:	printf( "WM_PENWINLAST(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_POWER:	printf( "WM_POWER(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_POWERBROADCAST:	printf( "WM_POWERBROADCAST(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_PRINT:	printf( "WM_PRINT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_PRINTCLIENT:	printf( "WM_PRINTCLIENT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_QUERYDRAGICON:	printf( "WM_QUERYDRAGICON(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_QUERYENDSESSION:	printf( "WM_QUERYENDSESSION(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_QUERYNEWPALETTE:	printf( "WM_QUERYNEWPALETTE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_QUERYOPEN:	printf( "WM_QUERYOPEN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_QUEUESYNC:	printf( "WM_QUEUESYNC(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_QUIT:	printf( "WM_QUIT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_RENDERALLFORMATS:	printf( "WM_RENDERALLFORMATS(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_RENDERFORMAT:	printf( "WM_RENDERFORMAT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SIZECLIPBOARD:	printf( "WM_SIZECLIPBOARD(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SPOOLERSTATUS:	printf( "WM_SPOOLERSTATUS(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_STYLECHANGED:	printf( "WM_STYLECHANGED(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_STYLECHANGING:	printf( "WM_STYLECHANGING(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SYSCOLORCHANGE:	printf( "WM_SYSCOLORCHANGE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SYSDEADCHAR:	printf( "WM_SYSDEADCHAR(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_TCARD:	printf( "WM_TCARD(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_THEMECHANGED:	printf( "WM_THEMECHANGED(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_TIMECHANGE:	printf( "WM_TIMECHANGE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_TIMER:	printf( "WM_TIMER(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_UNDO:	printf( "WM_UNDO(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_USER:	printf( "WM_USER(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_USERCHANGED:	printf( "WM_USERCHANGED(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_VKEYTOITEM:	printf( "WM_VKEYTOITEM(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_VSCROLL:	printf( "WM_VSCROLL(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_VSCROLLCLIPBOARD:	printf( "WM_VSCROLLCLIPBOARD(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_KEYLAST:	printf( "WM_KEYLAST(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETFONT:	printf( "WM_SETFONT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETHOTKEY:	printf( "WM_SETHOTKEY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETICON:	printf( "WM_SETICON(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETREDRAW:	printf( "WM_SETREDRAW(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETTEXT:	printf( "WM_SETTEXT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETTINGCHANGE:	printf( "WM_SETTINGCHANGE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;

	case WM_SIZE:
//		printf( "WM_SIZE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );
		glViewport(0,0, LOWORD(lParam), HIWORD(lParam) );
		break;
		
//	case WM_SYSKEYUP:			printf( "WM_SYSKEYUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_SYSKEYDOWN:			printf( "WM_SYSKEYDOWN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_PAINT:				printf( "WM_PAINT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );		break;
//	case WM_SYNCPAINT:	printf( "WM_SYNCPAINT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCDESTROY:			printf( "WM_NCDESTROY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_EXITSIZEMOVE:		printf( "WM_EXITSIZEMOVE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_WINDOWPOSCHANGING:	printf( "WM_WINDOWPOSCHANGING(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_WINDOWPOSCHANGED:	printf( "WM_WINDOWPOSCHANGED(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_KILLFOCUS:			printf( "WM_KILLFOCUS(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCACTIVATE:			printf( "WM_NCACTIVATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ACTIVATEAPP:		printf( "WM_ACTIVATEAPP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;

//	case WM_GETICON:			printf( "WM_GETICON(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCCREATE:			printf( "WM_NCCREATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_SIZE:				printf( "WM_SIZE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_SIZING:				printf( "WM_SIZING(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCCALCSIZE:			printf( "WM_NCCALCSIZE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCLBUTTONDBLCLK:	printf( "WM_NCLBUTTONDBLCLK(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MOVE:				printf( "WM_MOVE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MOVING:				printf( "WM_MOVING(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCLBUTTONDOWN:		printf( "WM_NCLBUTTONDOWN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_SYSCOMMAND:			printf( "WM_SYSCOMMAND(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_CAPTURECHANGED:		printf( "WM_CAPTURECHANGED(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_GETMINMAXINFO:		printf( "WM_GETMINMAXINFO(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ENTERSIZEMOVE:		printf( "WM_ENTERSIZEMOVE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ERASEBKGND:			printf( "WM_ERASEBKGND(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCPAINT:			printf( "WM_NCPAINT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MOUSEACTIVATE:		printf( "WM_MOUSEACTIVATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ACTIVATE:			printf( "WM_ACTIVATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_SETFOCUS:			printf( "WM_SETFOCUS(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_GETTEXT:			printf( "WM_GETTEXT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MDIACTIVATE:		printf( "WM_MDIACTIVATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_CONTEXTMENU:		printf( "WM_CONTEXTMENU(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCMOUSEMOVE:		printf( "WM_NCMOUSEMOVE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCHITTEST:			printf( "WM_NCHITTEST(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_SETCURSOR:			printf( "WM_SETCURSOR(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MOUSEMOVE:			printf( "WM_MOUSEMOVE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_LBUTTONDOWN:		printf( "WM_LBUTTONDOWN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_LBUTTONUP:			printf( "WM_LBUTTONUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_LBUTTONDBLCLK:		printf( "WM_LBUTTONDBLCLK(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_RBUTTONDOWN:		printf( "WM_RBUTTONDOWN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_RBUTTONUP:			printf( "WM_RBUTTONUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_RBUTTONDBLCLK:		printf( "WM_RBUTTONDBLCLK(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MBUTTONDOWN:		printf( "WM_MBUTTONDOWN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MBUTTONUP:			printf( "WM_MBUTTONUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MBUTTONDBLCLK:		printf( "WM_MBUTTONDBLCLK(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MOUSEWHEEL:			printf( "WM_MOUSEWHEEL(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MOUSEHOVER:			printf( "WM_MOUSEHOVER(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MOUSELEAVE:			printf( "WM_MOUSELEAVE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_SYSCHAR:			printf( "WM_SYSCHAR(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MENUCHAR:			printf( "WM_MENUCHAR(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_EXITMENULOOP:		printf( "WM_EXITMENULOOP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ENTERMENULOOP:		printf( "WM_ENTERMENULOOP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_INITMENU:			printf( "WM_INITMENU(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MENUSELECT:			printf( "WM_MENUSELECT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ENTERIDLE:			printf( "WM_ENTERIDLE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_CHAR:				printf( "WM_CHAR(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;

	case WM_KEYUP:				/*printf( "WM_KEYUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;*/
		key_update();
		break;

	case WM_KEYDOWN:			/*printf( "WM_KEYDOWN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );*/
		key_update();

		if( wParam == VK_F5 )
		{
			printf("VK_F5\n" );
		}

		if( wParam == 'C' )
		{
			if( GetKeyState( VK_CONTROL ) & 0x80 )
			{
				SendMessage(hWnd , WM_CLOSE , 0 , 0);
			}
		}
		return	0;
		break;

	case WM_DESTROY:
		printf("WM_DESTROY> %d\n", __LINE__ );
		PostQuitMessage( 0 );
		break;
	default:
		break;
	}


/*
	the process of the end;
		message:
		WM_CLOSE
		WM_WINDOWPOSCHANGING
		WM_WINDOWPOSCHANGED
		WM_NCACTIVATE
		WM_ACTIVATE
		WM_ACTIVATEAPP
		WM_KILLFOCUS
		WM_DESTROY
		WM_NCDESTROY
*/


	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
