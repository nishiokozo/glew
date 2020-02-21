#include <windows.h>
//#include <GL/gl.h>
//  #include <windows.h>
  #include <stdio.h>
  #include <math.h>
  #include <GL/glew.h>
  #include <GL/wglew.h>
//  #include <AL/al.h>
//  #include <AL/alut.h>
#include "loaderbmp.h"
#include "sys.h"

//-----------------------------------------------------------------------------
void	file_Release( void* pBuf )
//-----------------------------------------------------------------------------
{
	free( pBuf );
}
//-----------------------------------------------------------------------------
bool	file_Load( const char* fname, void** ppBuf, int* pLen )
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
	GLuint	 hdl,
	const void* pBuf, 
	int 		size
)
{
	int	ret = true;
 
	GLint compiled;

	// シェーダオブジェクトにプログラムをセット 
	glShaderSource( hdl, 1, (const char **)&pBuf, &size);

	// シェーダのコンパイル 
	glCompileShader( hdl );
	glGetShaderiv( hdl, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog( hdl );		// コンパイルログの出力 
	if (compiled == GL_FALSE)
	{
		fprintf(stderr, "Compile error in %s!!\n", "ffff");
		ret = false;
	//			return false;
	}

	return	ret;
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

	int	min = GL_NEAREST;
	int	mag = GL_NEAREST;
	int	wrap = GL_CLAMP_TO_EDGE;

	glGenTextures(1, &m_color_tex);
	glBindTexture(GL_TEXTURE_2D, m_color_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &m_depth_tex);
	glBindTexture(GL_TEXTURE_2D, m_depth_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
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
		-0.5f, -0.4f,	0.0f,
		 0.4f, -0.5f,	0.0f,
		 0.5f,  0.4f,	0.0f,
		-0.4f,  0.5f,	0.0f,
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

	sys.shader_Compile( m_hdlProgram, GL_VERTEX_SHADER, "constant-col.vs" ) ;
	sys.shader_Compile( m_hdlProgram, GL_FRAGMENT_SHADER, "constant-col.fs" ) ;

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
void DrawCol()
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
		-1.0f, -1.0f,	0.0f,
		 1.0f, -1.0f,	0.0f,
		 1.0f,  1.0f,	0.0f,
		-1.0f,  1.0f,	0.0f,
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

	sys.shader_Compile( m_hdlProgram, GL_VERTEX_SHADER, "constant-tex.vs" ) ;
	sys.shader_Compile( m_hdlProgram, GL_FRAGMENT_SHADER, "constant-tex.fs" ) ;

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

	//---

	LoaderBmp	pic( "dra.bmp" );
printf("pic %d %d\n", pic.width(), pic.height() );
	{
		int	width = pic.width();
		int	height = pic.height();

		int*	pPic = (int*)pic.address();

		float*	pBuf = (float*)malloc( 4*width*height*sizeof(float) );
		float*	p = pBuf;
		for ( int y = 0 ; y < height ; y++ )
		{
			for ( int x = 0 ; x < width ; x++ )
			{
				if ( x >= 64 && y >= 64 && x <64+2 && y < 64+2 )
				{
					p[0] = 1.0;
					p[1] = 1.0;
					p[2] = 1.0;
					p[3] = 1.0;
					p+=4;
					pPic++;
				}
				else
				{
					int	r = (((*pPic)>> 0)&0xff);
					int	g = (((*pPic)>> 8)&0xff);
					int	b = (((*pPic)>>16)&0xff);
					int	a = (((*pPic)>>24)&0xff);
					p[0] = (float)r/256.0f;
					p[1] = (float)g/256.0f;
					p[2] = (float)b/256.0f;
					p[3] = (float)a/256.0f;
					p+=4;
					pPic++;
				}
			}
		}
		
		int	min = GL_NEAREST;
		int	mag = GL_NEAREST;
		int	wrap = GL_REPEAT;

		glGenTextures( 1, &m_hdl_idTexture );
		glBindTexture( GL_TEXTURE_2D, m_hdl_idTexture );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, pBuf );
		glGenerateMipmap( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, 0 );
		
		free(pBuf);
	}

}
//------------------------------------------------------------------------------
void DrawTex()
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

class	ModelDot
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
ModelDot()
//------------------------------------------------------------------------------
{
	//-- create vertex buffer

	float	tblVertex[] =
	{
		-1.0f, -1.0f,	0.0f,
		 1.0f, -1.0f,	0.0f,
		 1.0f,  1.0f,	0.0f,
		-1.0f,  1.0f,	0.0f,
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

	sys.shader_Compile( m_hdlProgram, GL_VERTEX_SHADER, "constant-tex.vs" ) ;
	sys.shader_Compile( m_hdlProgram, GL_FRAGMENT_SHADER, "constant-tex.fs" ) ;

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

	//---

	{
		int	width = 256;
		int	height = 256;
		float*	pBuf = (float*)malloc( 4*width*height*sizeof(float) );
		float*	p = pBuf;
		for ( int y = 0 ; y < height ; y++ )
		{
			for ( int x = 0 ; x < width ; x++ )
			{
				if ( x >= 64 && y >= 64 && x <64+2 && y < 64+2 )
				{
					p[0] = 1.0;
					p[1] = 1.0;
					p[2] = 1.0;
					p[3] = 1.0;
					p+=4;
				}
				else
				{
					p[0] = 0.0;
					p[1] = 0.0;
					p[2] = 0.0;
					p[3] = 0.0;
					p+=4;
				}
			}
		}
		
		int	min = GL_NEAREST;
		int	mag = GL_NEAREST;
		int	wrap = GL_REPEAT;

		glGenTextures( 1, &m_hdl_idTexture );
		glBindTexture( GL_TEXTURE_2D, m_hdl_idTexture );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, pBuf );
		glGenerateMipmap( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, 0 );
		
		free(pBuf);
	}

}
//------------------------------------------------------------------------------
void DrawDot()
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

class	ModelFilter
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
ModelFilter()
//------------------------------------------------------------------------------
{
	//-- create vertex buffer

	float	tblVertex[] =
	{
		-1.0f, -1.0f,	0.0f,
		 1.0f, -1.0f,	0.0f,
		 1.0f,  1.0f,	0.0f,
		-1.0f,  1.0f,	0.0f,
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

	sys.shader_Compile( m_hdlProgram, GL_VERTEX_SHADER, "constant-tex.vs" ) ;
	sys.shader_Compile( m_hdlProgram, GL_FRAGMENT_SHADER, "constant-tex.fs" ) ;

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
void DrawFilter( int tex)
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

class	ModelGaussian
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
int		m_hdl_Length;
int		m_hdl_Weight;
int		m_hdl_Offset;

//------------------------------------------------------------------------------
ModelGaussian( const char* fnVs, const char* fnFs )
//------------------------------------------------------------------------------
{
	//-- create vertex buffer

	float	tblVertex[] =
	{
		-1.0f, -1.0f,	0.0f,
		 1.0f, -1.0f,	0.0f,
		 1.0f,  1.0f,	0.0f,
		-1.0f,  1.0f,	0.0f,
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

	sys.shader_Compile( m_hdlProgram, GL_VERTEX_SHADER, fnVs ) ;
	sys.shader_Compile( m_hdlProgram, GL_FRAGMENT_SHADER, fnFs ) ;

	glLinkProgram( m_hdlProgram );
	glValidateProgram( m_hdlProgram );


	m_hdl_Position		= glGetAttribLocation( m_hdlProgram, "Position");
	m_hdl_Uv			= glGetAttribLocation( m_hdlProgram, "Uv");
	m_hdl_Projection	= glGetUniformLocation( m_hdlProgram, "Projection");
	m_hdl_Texture		= glGetUniformLocation( m_hdlProgram, "Texture");
	m_hdl_Length		= glGetUniformLocation( m_hdlProgram, "Length");
	m_hdl_Weight		= glGetUniformLocation( m_hdlProgram, "Weight");
	m_hdl_Offset		= glGetUniformLocation( m_hdlProgram, "ofset");

	printf("hdl %d %d %d %d %d %d %d\n", 
		m_hdl_Position,
		m_hdl_Uv	,
		m_hdl_Projection,
		m_hdl_Texture	,
		m_hdl_Length	,
		m_hdl_Weight	,
		m_hdl_Offset	
	);
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
void DrawGaussian( int tex, float pitch, int lenWeight, float* tblWeight )
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

	glUniform1f(m_hdl_Offset, pitch );
	glUniform1i(m_hdl_Length, 10 );
	glUniform1fv(m_hdl_Weight, lenWeight, tblWeight );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glDrawElements( GL_TRIANGLE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );

}

};

//------------------------------------------------------------------------------
int main()
//------------------------------------------------------------------------------
{
#define	SIZE	512


	Fbo		fbo1(SIZE,SIZE);
	Fbo		fbo2(SIZE,SIZE);
	ModelDot	mdldot;

	ModelTex	mdltex;
	ModelCol	mdlbox;
	ModelFilter	filter;
	ModelGaussian	gaussianv( "gaussian-v.vs", "gaussian-v.fs" );
	ModelGaussian	gaussianh( "gaussian-h.vs", "gaussian-h.fs" );

	
	while ( sys.Update(1.0/60.0) )
	{

		glBindFramebuffer(GL_FRAMEBUFFER, fbo1.m_hdlFbo );
	 	glViewport( 0, 0, SIZE, SIZE );
		
		glClearColor( 0.3f, 0.2f, 0.1f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT );
	    glDisable(GL_DEPTH_TEST);

		mdldot.DrawDot();
		mdlbox.DrawCol();

		mdltex.DrawTex();

if(1)
		{
			int		lenWeight = 100;
			float	tblWeight[ lenWeight ];
		
			double 	s = 8.0;
			double 	sum = 0;
			for ( int i = 0 ; i < lenWeight ; i++ )
			{
				double x = (float)i;
				double g = 1.0/(sqrt(2.0*M_PI)*s)*exp(-x*x/(2.0*s*s));
				tblWeight[i] = g;
				sum += i?g*2:g;
			}


			tblWeight[0] /= sum;
			for ( int i = 1 ; i < lenWeight ; i++ )
			{
				 tblWeight[i] /= sum;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, fbo2.m_hdlFbo );
			gaussianv.DrawGaussian( fbo1.m_color_tex, 1.0f/SIZE, lenWeight, tblWeight );

			glBindFramebuffer(GL_FRAMEBUFFER, fbo1.m_hdlFbo );
			gaussianh.DrawGaussian( fbo2.m_color_tex, 1.0f/SIZE, lenWeight, tblWeight );

		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 	glViewport( 0, 0, SIZE, SIZE );

		glClearColor( 0.0f, 0.0f, 1.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		filter.DrawFilter( fbo1.m_color_tex );

	}

	return( 0 );
}



