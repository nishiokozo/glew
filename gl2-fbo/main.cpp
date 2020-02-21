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

} plat(512,512);



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


class	Fbo
{
public:
	GLuint	m_hdlFbo;
	GLuint	m_color_tex;
	GLuint	m_depth_tex;

	int		m_w;
	int		m_h;

//-----------------------------------------------------------------------------
Fbo( int w, int h )
//-----------------------------------------------------------------------------
{
	memset( this, 0, sizeof(*this));
	
	m_w = w;
	m_h = h;

	glGenTextures(1, &m_color_tex);
	glBindTexture(GL_TEXTURE_2D, m_color_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &m_depth_tex);
	glBindTexture(GL_TEXTURE_2D, m_depth_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	//-------------------------
	glGenFramebuffers(1, &m_hdlFbo);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_hdlFbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_color_tex, 0/*mipmap level*/);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_depth_tex, 0/*mipmap level*/);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);

}

};




class	ModelCol
{
public:

	int		m_cntIndex;
	float	m_matProjection[16]; 
	GLuint	m_hdlVertex_Position;
	GLuint	m_hdlVertex_Index;
	int		m_hdlProgram;
	int		m_hdl_Position;
	int		m_hdl_Projection;

//------------------------------------------------------------------------------
ModelCol()
//------------------------------------------------------------------------------
{
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

	const char*	vs_fn = "constant-col.vs";
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

	const char*	fs_fn = "constant-col.fs";
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
	m_hdl_Projection	= glGetUniformLocation( m_hdlProgram, "Projection");

	//---
	
	float	matProjection[] = 
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	for ( int i = 0 ; i < 16 ; i++ )
	{
		m_matProjection[i] = matProjection[i] ;
	}

}
//------------------------------------------------------------------------------
void Draw()
//------------------------------------------------------------------------------
{
	glUseProgram( m_hdlProgram );

	glUniformMatrix4fv( m_hdl_Projection, 1, GL_FALSE, m_matProjection );

	glEnableVertexAttribArray( m_hdl_Position );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Position  );
	glVertexAttribPointer( m_hdl_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glDrawElements( GL_TRIANGLE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );

}

};

class	ModelTex
{
public:

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
	
//------------------------------------------------------------------------------
ModelTex()
//------------------------------------------------------------------------------
{
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
		int	width = 2;
		int	height = 2;
		float	buf[] = 
		{
			1.0, 0.0, 0.0, 1.0,		0.0, 0.0, 1.0, 1.0, 
			0.0, 1.0, 0.0, 1.0,		1.0, 1.0, 1.0, 1.0, 
		};
		int	min = GL_LINEAR_MIPMAP_LINEAR;
		int	mag = GL_LINEAR;
		int	wrap = GL_REPEAT;

		glGenTextures( 1, &m_hdl_idTexture );
		glBindTexture( GL_TEXTURE_2D, m_hdl_idTexture );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, buf );
		glGenerateMipmap( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

}
//------------------------------------------------------------------------------
void Draw()
//------------------------------------------------------------------------------
{
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

}

};
class	ModelFbo
{
public:

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
	
//------------------------------------------------------------------------------
ModelFbo()
//------------------------------------------------------------------------------
{
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
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	for ( int i = 0 ; i < 16 ; i++ )
	{
		m_matProjection[i] = matProjection[i] ;
	}


}
//------------------------------------------------------------------------------
void Draw( int tex)
//------------------------------------------------------------------------------
{
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
	glBindTexture( GL_TEXTURE_2D, tex );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glDrawElements( GL_TRIANGLE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );

}

};

//------------------------------------------------------------------------------
int main()
//------------------------------------------------------------------------------
{
	ModelCol	mdl1;
	ModelTex	mdl2;
	ModelFbo	mdl3;

	Fbo	fbo(512,512);

	while( plat.update() )
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo.m_hdlFbo );
		
		glClearColor( 0.3f, 0.2f, 0.1f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		mdl1.Draw();
		mdl2.Draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor( 0.0f, 0.0f, 1.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		mdl3.Draw( fbo.m_color_tex );

	}

	return( 0 );
}



