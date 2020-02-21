#include <windows.h>
//#include <GL/gl.h>
//  #include <windows.h>
  #include <stdio.h>
  #include <GL/glew.h>
  #include <GL/wglew.h>
//  #include <AL/al.h>
//  #include <AL/alut.h>


//------------------------------------------------------------------------------
double	GetSecond()
//------------------------------------------------------------------------------
{
	LARGE_INTEGER frequency; 
	QueryPerformanceFrequency(&frequency);
	double f = frequency.QuadPart;
//	double f = 2741201;
//printf( "%d\n", frequency.QuadPart ) ;
	LARGE_INTEGER tm;
	QueryPerformanceCounter(&tm);

	return (double)(tm.QuadPart) / f;

}

//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc
//------------------------------------------------------------------------------
(
	  HWND	m_hWnd
	, UINT	uMsg
	, WPARAM wParam
	, LPARAM lParam
)
{
	switch( uMsg ) 
	{
	case WM_CREATE:
			break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	}

	return DefWindowProc( m_hWnd, uMsg, wParam, lParam );
}

//------------------------------------------------------------------------------
void EnableOpenGL(HWND m_hWnd, HDC * hDC, HGLRC * hGLRC)
//------------------------------------------------------------------------------
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	// get the device context (DC)
	*hDC = GetDC( m_hWnd );
	
	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( *hDC, &pfd );
	SetPixelFormat( *hDC, format, &pfd );
	
	// create and enable the render context (RC)
	*hGLRC = wglCreateContext( *hDC );
	wglMakeCurrent( *hDC, *hGLRC );

	// GLEW初期化
	{
		int err = glewInit();
		if (err != GLEW_OK)
		{
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
	}

	wglSwapIntervalEXT(0);
	
}

//------------------------------------------------------------------------------
void DisableOpenGL(HWND m_hWnd, HDC hDC, HGLRC hGLRC)
//------------------------------------------------------------------------------
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hGLRC );
	ReleaseDC( m_hWnd, hDC );
}

class	Plat
{
	public:
	boolean	m_bError;
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hGLRC;

//------------------------------------------------------------------------------
Plat( int width, int height )
//------------------------------------------------------------------------------
{
	HINSTANCE	hInstance;
	hInstance	 = GetModuleHandle( NULL );

	WNDCLASSEX wc;
	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;  
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground 	= (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName  	= MAKEINTRESOURCE( NULL );
	wc.lpszClassName 	= "MainWindowClass";
	wc.hIconSm			= NULL;

	if ( 0 == RegisterClassEx( &wc ) ) 
	{
		m_bError = true;
	}
	else
	{
		m_hWnd = CreateWindowEx (
			  WS_EX_APPWINDOW
			, wc.lpszClassName
			, "yukizo " __DATE__
			, WS_OVERLAPPEDWINDOW
			, CW_USEDEFAULT
			, CW_USEDEFAULT
			, width
			, height
			, NULL
			, NULL
			, hInstance
			, 0
		);
		EnableOpenGL( m_hWnd, &m_hDC, &m_hGLRC );

		ShowWindow (m_hWnd, SW_NORMAL);
	}
}

//------------------------------------------------------------------------------
~Plat()
//------------------------------------------------------------------------------
{
	DisableOpenGL( m_hWnd, m_hDC, m_hGLRC );

	DestroyWindow( m_hWnd );
}

//------------------------------------------------------------------------------
boolean	update()
//------------------------------------------------------------------------------
{
	MSG msg;

	boolean flgLoop = true;
		
	if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
	{
		
		if ( msg.message == WM_QUIT ) 
		{
			flgLoop = true;
		} 
		else 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		
	} 
	else 
	{
	}

	return	flgLoop;	
}

} plat(640,480);



// シェーダコンパイルのログバッファ 
#define MAX_SHADER_LOG_SIZE		(1024)
static	char s_logBuffer[MAX_SHADER_LOG_SIZE]; 
//-----------------------------------------------------------------------------
static void printShaderInfoLog(
//-----------------------------------------------------------------------------
	GLuint shader
)
{
	int logSize;
	int length;

	// ログの長さは、最後のNULL文字も含む 
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &logSize);

	if (logSize > 1)
	{
		glGetShaderInfoLog(shader, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
//			fprintf(stderr, "DataShader Info Log\n%s\n", s_logBuffer);

			printf( "\n%s\n", s_logBuffer );
//				exit(0);

	}
}

//-----------------------------------------------------------------------------
bool loadShader(
//-----------------------------------------------------------------------------
	GLuint* 	pHdl,
	const void* pBuf, 
	int 		size
)
{
	int	ret = true;
 
	GLint compiled;

	// シェーダオブジェクトにプログラムをセット 
	glShaderSource(*pHdl, 1, (const char **)&pBuf, &size);

	// シェーダのコンパイル 
	glCompileShader(*pHdl);
	glGetShaderiv(*pHdl, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(*pHdl);		// コンパイルログの出力 
	if (compiled == GL_FALSE)
	{
		fprintf(stderr, "Compile error in %s!!\n", "ffff");
		ret = false;
	//			return false;
	}

	return	ret;
}

//-----------------------------------------------------------------------------
bool	file_load( const char* fname, void** ppBuf, int* pLen )
//-----------------------------------------------------------------------------
{
	void*	fbuf = 0;
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
			}
			{
				fbuf = malloc( flen );
				fseek(fp, 0, SEEK_SET);
				fread(fbuf, 1, flen, fp);
			}
			fclose( fp );
		}
	}
	(*ppBuf) = fbuf;
	(*pLen) = flen;

	return	true;
}

	GLuint g_hdlVertexPosition;
	GLuint g_hdlVertexIndex;
	int	g_hdlProgram;
	int	g_hdl_Position;
	int	g_hdl_Projection;

//------------------------------------------------------------------------------
void render()
//------------------------------------------------------------------------------
{
	static	float theta = 0.0f;
			
	
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	
	glPushMatrix();
	glRotatef( theta, 0.0f, 0.0f, 1.0f );
	glBegin( GL_TRIANGLES );
	glColor3f( 1.0f, 0.0f, 0.0f ); glVertex2f( 0.0f, 1.0f );
	glColor3f( 0.0f, 1.0f, 0.0f ); glVertex2f( 0.87f, -0.5f );
	glColor3f( 0.0f, 0.0f, 1.0f ); glVertex2f( -0.87f, -0.5f );
	glEnd();
	glPopMatrix();
	
	theta += 1.0f;

}


//------------------------------------------------------------------------------
int main()
//------------------------------------------------------------------------------
{
	//-- create vertex buffer

	float	tblVertex[] =
	{
		0.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
	};
	glGenBuffers( 1, &g_hdlVertexPosition );
	glBindBuffer(GL_ARRAY_BUFFER, g_hdlVertexPosition );
	glBufferData(GL_ARRAY_BUFFER, sizeof(tblVertex), tblVertex, GL_STATIC_DRAW  );

	int	tblIncex[] =
	{
		0,1,2
	};
	glGenBuffers(1, &g_hdlVertexIndex );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_hdlVertexIndex );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tblIncex), tblIncex, GL_STATIC_DRAW);

	//-- create shader

	g_hdlProgram = glCreateProgram();

	const char*	vs_fn = "flat.vs";
	void*	vs_pBuf = 0;
	int		vs_len = 0;
	{
		const char*	fn		= vs_fn;
		void**	ppBuf	= &vs_pBuf;
		int		len		= vs_len;
		file_load( fn, ppBuf, &len );
		GLuint hw_hdl = glCreateShader(GL_VERTEX_SHADER);
		loadShader( &hw_hdl, *ppBuf, len );
		glAttachShader( g_hdlProgram, hw_hdl);
		glDeleteShader(hw_hdl);
	}

	const char*	fs_fn = "flat.fs";
	void*	fs_pBuf = 0;
	int		fs_len = 0;
	{
		const char*	fn		= fs_fn;
		void**	ppBuf	= &fs_pBuf;
		int		len		= fs_len;
		file_load( fn, ppBuf, &len );
		GLuint hw_hdl = glCreateShader(GL_FRAGMENT_SHADER);
		loadShader( &hw_hdl, *ppBuf, len );
		glAttachShader( g_hdlProgram, hw_hdl);
		glDeleteShader(hw_hdl);
	}
	glLinkProgram( g_hdlProgram );
	glValidateProgram( g_hdlProgram );


	g_hdl_Position		= glGetAttribLocation( g_hdlProgram, "Position");
	g_hdl_Projection	= glGetUniformLocation( g_hdlProgram, "Projection");

	//---
	
	float	matProjection[] = 
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	while( plat.update() )
	{
//		render();
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glUseProgram( g_hdlProgram );

		glUniformMatrix4fv( g_hdl_Projection, 1, GL_FALSE, matProjection );
	
		glEnableVertexAttribArray( g_hdl_Position );
		glBindBuffer(GL_ARRAY_BUFFER, g_hdlVertexPosition  );
		glVertexAttribPointer( g_hdl_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_hdlVertexIndex );
		glDrawElements( GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, 0 );


		SwapBuffers( plat.m_hDC );

static	double	a0 = GetSecond();
static	double	a1 = GetSecond();
a1 = GetSecond();
//printf("int %f\n", a1-a0 );
a0 = GetSecond();

	}

	return( 0 );
}



