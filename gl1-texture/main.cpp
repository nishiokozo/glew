#include <windows.h>
//#include <GL/gl.h>
//  #include <windows.h>
  #include <stdio.h>
  #include <GL/glew.h>
  #include <GL/wglew.h>
//  #include <AL/al.h>
//  #include <AL/alut.h>


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
void EnableOpenGL(HWND m_hWnd, HDC * pHDC, HGLRC * hGLRC)
//------------------------------------------------------------------------------
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	// get the device context (DC)
	(*pHDC) = GetDC( m_hWnd );
	
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

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	
}

//------------------------------------------------------------------------------
void DisableOpenGL(HWND m_hWnd, HDC hDC, HGLRC hGLRC)
//------------------------------------------------------------------------------
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hGLRC );
	ReleaseDC( m_hWnd, hDC );

}

class	Win
{
	public:
	boolean	m_bError;
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hGLRC;

//------------------------------------------------------------------------------
Win( int width, int height )
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
~Win()
//------------------------------------------------------------------------------
{
	DisableOpenGL( m_hWnd, m_hDC, m_hGLRC );

	DestroyWindow( m_hWnd );
}

//------------------------------------------------------------------------------
boolean	 update()
//------------------------------------------------------------------------------
{
	int	flgLoop = false;

	while (true)
	{
		MSG	msg;
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE) )
		{
			if (msg.message == WM_QUIT)
			{
				flgLoop = false;
				break;
			}
			else
			{
				TranslateMessage( &msg );
				DispatchMessage(&msg);
			}
		}
		else
		{
			SwapBuffers( m_hDC );

			flgLoop = true;
			break;
		}
	}

	return	flgLoop;	
}

};



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

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &logSize);

	if (logSize > 1)
	{
		glGetShaderInfoLog(shader, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);

			printf( "\n%s\n", s_logBuffer );

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





//------------------------------------------------------------------------------
int main()
//------------------------------------------------------------------------------
{
	Win win(612,512);

	int		m_cntIndex;
	float	m_matProjection[16]; 
	GLuint	m_hdlVertex_Position;
	GLuint	m_hdlVertex_Uv;
	GLuint	m_hdlVertex_Index;
	GLuint	m_hdlProgram;
	int		m_hdl_Position;
	int		m_hdl_Uv;
	int		m_hdl_Projection;
	int		m_hdl_Texture;
	GLuint	m_hdl_idTexture;

	//-- create vertex buffer

	float	tblVertex[] =
	{
		-0.5f, -0.5f,	0.0f,
		 0.5f, -0.5f,	0.0f,
		 0.5f,  0.5f,	0.0f,
		-0.5f,  0.5f,	0.0f,
	};
	glGenBuffers( 1, &m_hdlVertex_Position );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Position );
	glBufferData(GL_ARRAY_BUFFER, sizeof(tblVertex), tblVertex, GL_STATIC_DRAW  );

	float	tblUv[] =
	{
		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,
	};
	glGenBuffers( 1, &m_hdlVertex_Uv );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Uv );
	glBufferData(GL_ARRAY_BUFFER, sizeof(tblUv), tblUv, GL_STATIC_DRAW  );

	int	tblIndex[] =
	{
		0,1,2,
		2,3,0,
	};
	glGenBuffers(1, &m_hdlVertex_Index );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tblIndex), tblIndex, GL_STATIC_DRAW);


	m_cntIndex = sizeof(tblIndex)/sizeof(int);

	//-- create shader

	m_hdlProgram = glCreateProgram();

	const char*	vs_fn = "constant-tex.vs";
	void*	vs_pBuf = 0;
	int		vs_len = 0;
	{
		const char*	fn		= vs_fn;
		void**	ppBuf	= &vs_pBuf;
		int		len		= vs_len;
		file_load( fn, ppBuf, &len );
		GLuint hw_hdl = glCreateShader(GL_VERTEX_SHADER);
		loadShader( &hw_hdl, *ppBuf, len );
		glAttachShader( m_hdlProgram, hw_hdl);
		glDeleteShader(hw_hdl);
	}

	const char*	fs_fn = "constant-tex.fs";
	void*	fs_pBuf = 0;
	int		fs_len = 0;
	{
		const char*	fn		= fs_fn;
		void**	ppBuf	= &fs_pBuf;
		int		len		= fs_len;
		file_load( fn, ppBuf, &len );
		GLuint hw_hdl = glCreateShader(GL_FRAGMENT_SHADER);
		loadShader( &hw_hdl, *ppBuf, len );
		glAttachShader( m_hdlProgram, hw_hdl);
		glDeleteShader(hw_hdl);
	}
	glLinkProgram( m_hdlProgram );
	glValidateProgram( m_hdlProgram );


	m_hdl_Position		= glGetAttribLocation( m_hdlProgram, "Position");
	m_hdl_Uv			= glGetAttribLocation( m_hdlProgram, "Uv");
	m_hdl_Projection	= glGetUniformLocation( m_hdlProgram, "Projection");
	m_hdl_Texture		= glGetUniformLocation( m_hdlProgram, "Texture");

	//---
	
	float	matProjection[] = 
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.2f, 0.2f, 0.0f, 1.0f,
	};
	for ( int i = 0 ; i < 16 ; i++ )
	{
		m_matProjection[i] = matProjection[i] ;
	}


	{
		int	width = 400;
		int	height = 400;
		int	min = GL_LINEAR;
		int	mag = GL_LINEAR;
		int	wrap = GL_REPEAT;

		glGenTextures( 1, &m_hdl_idTexture );
		glBindTexture( GL_TEXTURE_2D, m_hdl_idTexture );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0 );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	{
		int	width = 2;
		int	height = 2;
		float	buf[] = 
		{
			1.0, 0.0, 0.0, 1.0,		0.0, 0.0, 1.0, 1.0, 
			0.0, 1.0, 0.0, 1.0,		1.0, 1.0, 1.0, 1.0, 
		};

		glBindTexture( GL_TEXTURE_2D, m_hdl_idTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, buf );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}


	GLuint	m_hdlFbo;
	GLuint	m_hdl_tex;

	glGenFramebuffers(1, &m_hdlFbo);
	{
		int	width = 512;
		int	height = 512;
		int	min = GL_LINEAR;
		int	mag = GL_LINEAR;
		int	wrap = GL_REPEAT;
		glGenTextures( 1, &m_hdl_tex );
		glBindTexture( GL_TEXTURE_2D, m_hdl_tex );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0 );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	{
		int	width = 2;
		int	height = 2;
		float	buf[] = 
		{
			1.0, 0.0, 0.0, 1.0,		0.0, 0.0, 1.0, 1.0, 
			0.0, 1.0, 0.0, 1.0,		1.0, 1.0, 1.0, 1.0, 
		};

		glBindTexture( GL_TEXTURE_2D, m_hdl_tex );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, buf );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	while( win.update() )
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_hdlFbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hdl_tex, 0/*mipmap level*/);
	

		glClearColor( 0.0f, 0.0f, 1.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glUseProgram( m_hdlProgram );

		glUniformMatrix4fv( m_hdl_Projection, 1, GL_FALSE, m_matProjection );

		glEnableVertexAttribArray( m_hdl_Position );
		glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Position  );
		glVertexAttribPointer( m_hdl_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray( m_hdl_Uv );
		glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Uv  );
		glVertexAttribPointer( m_hdl_Uv, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glUniform1i( m_hdl_Texture, 0);
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_hdl_idTexture );

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
		glDrawElements( GL_TRIANGLE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );




		glBindFramebuffer(GL_FRAMEBUFFER, 0 );


		glClearColor( 1.0f, 0.0f, 0.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glUseProgram( m_hdlProgram );

		glUniformMatrix4fv( m_hdl_Projection, 1, GL_FALSE, m_matProjection );

		glEnableVertexAttribArray( m_hdl_Position );
		glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Position  );
		glVertexAttribPointer( m_hdl_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray( m_hdl_Uv );
		glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Uv  );
		glVertexAttribPointer( m_hdl_Uv, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glUniform1i( m_hdl_Texture, 0);
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_hdl_tex );

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
		glDrawElements( GL_TRIANGLE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );

	}

	return( 0 );
}



