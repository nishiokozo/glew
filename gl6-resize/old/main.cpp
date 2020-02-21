#include	<iostream>
#include <string>
using namespace std;

#include <windows.h>
//#include <GL/gl.h>
//  #include <windows.h>
  #include <stdio.h>
  #include <math.h>
  #include <ctype.h>
  #include <GL/glew.h>
  #include <GL/wglew.h>
//  #include <AL/al.h>
//  #include <AL/alut.h>
#include "loaderbmp.h"
#include "vec.h"
#include "mat.h"


//------------------------------------------------------------------------------
double	getQueryPerformanceCounter()
//------------------------------------------------------------------------------
{
   	LARGE_INTEGER frequency;        // ticks per second
    QueryPerformanceFrequency(&frequency);

   	LARGE_INTEGER t1;        // ticks per second
    QueryPerformanceCounter(&t1);

	return (t1.QuadPart) * 1000.0 / frequency.QuadPart;

}

//-----------------------------------------------------------------------------
void	file_Release( void* m_pBuf )
//-----------------------------------------------------------------------------
{
	free( m_pBuf );
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
bool shader_compile(
//-----------------------------------------------------------------------------
	int		hdlProgram,
	int		type,
	char*	fn
)
{
	GLint result;

	void*	m_pBuf = 0; 
	int		m_len = 0;

	file_Load( fn, &m_pBuf, &m_len );
	GLuint hdl = glCreateShader(type);

	glShaderSource( hdl, 1, (const char **)&m_pBuf, &m_len);
	glCompileShader( hdl );
	glGetShaderiv( hdl, GL_COMPILE_STATUS, &result);

	printShaderInfoLog( hdl );
	if (result == GL_FALSE)
	{
		fprintf(stderr, "Compile error at [%s]\n", fn);
	}

	glAttachShader( hdlProgram, hdl );
	glDeleteShader( hdl );
	file_Release( m_pBuf );

	return	true;
}




////////////////////////////////////////////////////////////////////////////////
class	ModelTex
////////////////////////////////////////////////////////////////////////////////
{
public:

	int		m_cntIndex;
	mat4	M; 
	float	m_matProjection[16]; 
	GLuint	m_hdlVertex_Position;
	GLuint	m_hdlVertex_Normal;
	GLuint	m_hdlVertex_Uv;
	GLuint	m_hdlVertex_Index;
	GLuint	m_hdlProgram;
	int		m_hdl_Position;
	int		m_hdl_Uv;
	int		m_hdl_Projection;
	int		m_hdl_Model;
	int		m_hdl_Texture;
	int		m_hdl_Normal;
	GLuint	m_hdl_idTexture;
	
//------------------------------------------------------------------------------
ModelTex()
//------------------------------------------------------------------------------
{

	int	tblIndex[] =
	{
		   0,   1,   2,   3,	  3,  4+1,
		 4+1, 4+0, 4+3, 4+2,    4+2,  8+0,
		 8+0, 8+1, 8+2, 8+3,	8+3, 12+1,
		12+1,12+0,12+3,12+2,   12+2, 16+0,
		16+0,16+1,16+2,16+3,   16+3, 20+1,
		20+1,20+0,20+3,20+2,
	};
	glGenBuffers(1, &m_hdlVertex_Index );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tblIndex), tblIndex, GL_STATIC_DRAW);

	m_cntIndex = sizeof(tblIndex)/sizeof(int);


	float	tblPosition[] =
	{
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,

		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,

	};
	glGenBuffers( 1, &m_hdlVertex_Position );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Position );
	glBufferData(GL_ARRAY_BUFFER, sizeof(tblPosition), tblPosition, GL_STATIC_DRAW  );

	float	tblNormal[] =
	{
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,

		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,

		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,

		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,

		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,

		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0,  0.0f,
	};
	glGenBuffers( 1, &m_hdlVertex_Normal );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Normal );
	glBufferData(GL_ARRAY_BUFFER, sizeof(tblNormal), tblNormal, GL_STATIC_DRAW  );

	float	tblUv[] =
	{
		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,

		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,

		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,

		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,

		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,

		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,
	};
	glGenBuffers( 1, &m_hdlVertex_Uv );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Uv );
	glBufferData(GL_ARRAY_BUFFER, sizeof(tblUv), tblUv, GL_STATIC_DRAW  );


	//-- create shader

	m_hdlProgram = glCreateProgram();

	shader_compile( m_hdlProgram, GL_VERTEX_SHADER, "s_PN.vs" ) ;
	shader_compile( m_hdlProgram, GL_FRAGMENT_SHADER, "s_PN.fs" ) ;

	glLinkProgram( m_hdlProgram );
	glValidateProgram( m_hdlProgram );


	m_hdl_Position		= glGetAttribLocation( m_hdlProgram, "Position");
	m_hdl_Uv			= glGetAttribLocation( m_hdlProgram, "Uv");
	m_hdl_Normal		= glGetAttribLocation( m_hdlProgram, "Normal");
	m_hdl_Projection	= glGetUniformLocation( m_hdlProgram, "Projection");
	m_hdl_Model			= glGetUniformLocation( m_hdlProgram, "M");
	m_hdl_Texture		= glGetUniformLocation( m_hdlProgram, "Texture");

printf(" shd %d %d\n", m_hdl_Position, m_hdl_Normal );

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

		float*	m_pBuf = (float*)malloc( 4*width*height*sizeof(float) );
		float*	p = m_pBuf;
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
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, m_pBuf );
		glGenerateMipmap( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, 0 );
		
		free(m_pBuf);
	}

	// --

	mat_identity( M.m_array );

}
//------------------------------------------------------------------------------
void DrawTex( )
//------------------------------------------------------------------------------
{
	
	glUseProgram( m_hdlProgram );

	glUniformMatrix4fv( m_hdl_Projection, 1, GL_TRUE, m_matProjection );
	glUniformMatrix4fv( m_hdl_Model,      1, GL_TRUE, M.m_array );

	glEnableVertexAttribArray( m_hdl_Position );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Position  );
	glVertexAttribPointer( m_hdl_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray( m_hdl_Normal );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Normal  );
	glVertexAttribPointer( m_hdl_Normal, 3, GL_FLOAT, GL_FALSE, 0, 0);

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

////////////////////////////////////////////////////////////////////////////////
class	ModelSphere
////////////////////////////////////////////////////////////////////////////////
{
public:

	int		m_cntIndex;
	mat4	m_matProjection; 
	mat4	M; 
	GLuint	m_hdlVertex_Position;
	GLuint	m_hdlVertex_Normal;
	GLuint	m_hdlVertex_Index;
	int		m_hdlProgram;
	int		m_hdl_Projection;
	int		m_hdl_Model;
	int		m_hdl_Position;
	int		m_hdl_Normal;

//------------------------------------------------------------------------------
ModelSphere()
//------------------------------------------------------------------------------
{
	struct	Position	{	float	x,y,z;	};
	struct	Normal		{	float	x,y,z;	};

	int	n = 10;
	int	sv = n*5;
	int	sh = n*10;
	int	cntVertex = sv*sh;
	{
		float	r = 1.0;

	  	Position*	tblPosition = (Position*)malloc( cntVertex*sizeof(Position) );
		Normal*		tblNormal = (Normal*)malloc( cntVertex*sizeof(Normal) );

		for ( int v = 0 ; v < sv ; v++ )
		{
			float th = (float)v/(float)(sv-1)*M_PI;
//if ( v > sv/3 ) th = 0;
			float y = r*cos(th);
			float l = r*sin(th);
			for ( int h = 0 ; h < sh ; h++ )
			{
				float th = (float)h/(float)(sh-1)*2*M_PI;
				float x = l*sin(th);
				float z = l*cos(th);

				int	j = v*sh+h;
				tblPosition[j].x = x;
				tblPosition[j].y = y;
				tblPosition[j].z = z;
				float l = sqrt( x*x + y*y + z*z );
				tblNormal[j].x = x/l;
				tblNormal[j].y = y/l;
				tblNormal[j].z = z/l;
			}
		}

		m_cntIndex = (sv-1)*sh*2;
	
		glGenBuffers( 1, &m_hdlVertex_Position );
		glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Position );
		glBufferData(GL_ARRAY_BUFFER, cntVertex*sizeof(Position), tblPosition, GL_STATIC_DRAW  );

		glGenBuffers( 1, &m_hdlVertex_Normal );
		glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Normal );
		glBufferData(GL_ARRAY_BUFFER, cntVertex*sizeof(Normal), tblNormal, GL_STATIC_DRAW  );

		free( tblPosition );
	}


	{
		int*	tblIndex = (int*)malloc( m_cntIndex*sizeof(int) );
		int	i = 0;
		for ( int v = 0 ; v < sv-1 ; v++ )
		{
			for ( int h = 0 ; h < sh ; h++ )
			{
				int	j = v*sh+h;
				tblIndex[i++] = j;
				tblIndex[i++] = j+sh;
			}
		}
		
		glGenBuffers(1, &m_hdlVertex_Index );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_cntIndex*sizeof(int), tblIndex, GL_STATIC_DRAW);

		free( tblIndex );
	}

	//--

	m_hdlProgram = glCreateProgram();
	shader_compile( m_hdlProgram, GL_VERTEX_SHADER, "s_PN.vs" ) ;
	shader_compile( m_hdlProgram, GL_FRAGMENT_SHADER, "s_PN.fs" ) ;
	glLinkProgram( m_hdlProgram );
	glValidateProgram( m_hdlProgram );
	m_hdl_Projection	= glGetUniformLocation( m_hdlProgram, "P");
	m_hdl_Model			= glGetUniformLocation( m_hdlProgram, "M");
	m_hdl_Position		= glGetAttribLocation( m_hdlProgram, "Position");
	m_hdl_Normal		= glGetAttribLocation( m_hdlProgram, "Normal");

	glBindFragDataLocation( m_hdlProgram, 0, "Color0");

	//---

	mat_identity( M.m_array );

	float	mat[] = 
	{
		 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f,
	};
	for ( int i = 0 ; i < 16 ; i++ )
	{
		M.m_array[i] = mat[i] ;
	}

/*
	float	x1 = -1;
	float	x2 =  1;
	float	y1 = -1;
	float	y2 =  1;
	float	z1 = -1.1;
	float	z2 = -100;

	float sx =    2.0f*z1 / (x2-x1);
	float sy =    2.0f*z1 / (y2-y1);
	float sz =    (z2+z1) / (z2-z1);

	float xz =    (z2+z1) / (z2-z1);
	float yz =    (z2+z1) / (z2-z1);
	float w = 1.0f / z1;
*/
	float	P[] = 
	{
		 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f, 0.0f,-22.0f, 1.0f,
	};
	for ( int i = 0 ; i < 16 ; i++ )
	{
		m_matProjection.m_array[i] = P[i] ;
	}


//mat_Frustum( m_matProjection, -1, 1, -1, 1, -1, -100 );	
/*
mat_print( M.m_array );
mat_print( m_matProjection.m_array );

printf("cos %f\n", cos(M_PI*45.0/180.0f) );
printf("acos %f\n", cosh(1.0/cos(M_PI*45.0/180.0f)) );
printf("acos %f\n", acos(1.0/cos(M_PI*45.0/180.0f)) );
printf("exp %f\n", exp(1) );
*/
}

//------------------------------------------------------------------------------
void DrawSphere()
//------------------------------------------------------------------------------
{


	glUseProgram( m_hdlProgram );


	glUniformMatrix4fv( m_hdl_Model,      1, GL_TRUE, M.m_array );
	glUniformMatrix4fv( m_hdl_Projection, 1, GL_TRUE, m_matProjection.m_array );

	glEnableVertexAttribArray( m_hdl_Position );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Position  );
	glVertexAttribPointer( m_hdl_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray( m_hdl_Normal );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Normal  );
	glVertexAttribPointer( m_hdl_Normal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glDrawElements( GL_TRIANGLE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );
//	glDrawElements( GL_LINE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );

}

};



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

	// Culling
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace( GL_CW );

	// Depth
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);	

	// Alpha
	glDisable(GL_ALPHA_TEST);
}

//------------------------------------------------------------------------------
void DisableOpenGL(HWND m_hWnd, HDC hDC, HGLRC hGLRC)
//------------------------------------------------------------------------------
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hGLRC );
	ReleaseDC( m_hWnd, hDC );

}

//static	HDC		m_hDC;
//static	HGLRC	m_hGLRC;




////////////////////////////////////////////////////////////////////////////////
class Environment
////////////////////////////////////////////////////////////////////////////////
{
public:
	void*	m_pBuf; 
	int		m_len;
	char	m_str[256];

//-----------------------------------------------------------------------------
~Environment()
//-----------------------------------------------------------------------------
{
	file_Release( &m_pBuf );
}

//-----------------------------------------------------------------------------
Environment()
//-----------------------------------------------------------------------------
{
	memset( this, 0, sizeof(*this) );

	file_Load( "win.ini", &m_pBuf, &m_len );

}
//-----------------------------------------------------------------------------
bool	FindValue( char* name )
//-----------------------------------------------------------------------------
{
	int		n = 0;
	int		cnt = 0;
	bool	flgFind = false;
	
	m_str[0] = 0;

	for ( int i = 0 ; i < m_len  ; i++  )
	{
		char	c = ((char*)m_pBuf)[i];

		bool	flgCtrl = ( iscntrl(c) || isspace(c) ) ;
		bool	flgEow = ( flgCtrl == true || i+1 == m_len ) ;
		bool	flgEol = ( c == 0x0a || i+1 == m_len ) ;

		if ( flgCtrl == false ) m_str[n++] = c;
			
		if ( flgEow )
		{
			m_str[n] = 0;
			n = 0;

			if ( cnt == 0 && strcmp( m_str, name ) == 0 ) 
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
	
	if ( flgFind == false ) m_str[0] = 0;

	return flgFind;
}

//-----------------------------------------------------------------------------
float	GetFloat( char* name )
//-----------------------------------------------------------------------------
{
	if ( FindValue( name ) ) return atof( m_str );
	return 0.0f;
}
//-----------------------------------------------------------------------------
int		GetInt( char* name )
//-----------------------------------------------------------------------------
{
	if ( FindValue( name ) ) return atoi( m_str );
	return 0;
}
//-----------------------------------------------------------------------------
string	GetString( char* name )
//-----------------------------------------------------------------------------
{
	FindValue( name );
	return m_str;
}

} env;


//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc
//------------------------------------------------------------------------------
(
	  HWND	hWnd
	, UINT	uMsg
	, WPARAM wParam
	, LPARAM lParam
);


////////////////////////////////////////////////////////////////////////////////
class	Win
////////////////////////////////////////////////////////////////////////////////
{
public:
	boolean	m_bError;
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hGLRC;

	void	(*m_pFuncPaint)();

	int	w;
	int	h;

//------------------------------------------------------------------------------
Win::~Win()
//------------------------------------------------------------------------------
{
	DisableOpenGL( m_hWnd, m_hDC, m_hGLRC );
	DestroyWindow( m_hWnd );
}


//------------------------------------------------------------------------------
Win::Win()
//------------------------------------------------------------------------------
{
	memset( this, 0, sizeof(*this) );

	w = env.GetInt("width"); 
	h = env.GetInt("height");

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
	wc.lpszClassName 	= "MainWindowClass";
	wc.hIconSm			= NULL;

	if ( 0 == RegisterClassEx( &wc ) ) 
	{
		m_bError = true;
	}
	else
	{

		{
			RECT rc;
			SetRect(&rc, 0, 0, w, h );
			AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, 0);

			m_hWnd = CreateWindowEx (
				// message:
				// 1)WM_GETMINMAXINFO
				// 2)WM_NCCREATE
				// 3)WM_NCCALCSIZE
				// 4)WM_CREATE
				  0
				, wc.lpszClassName
				, (env.GetString("name") + " " + string(__DATE__) ).c_str()
				, WS_OVERLAPPEDWINDOW   	// window style
				, 150+rc.left   	        // horizontal position of window
				, 150+rc.top				// vertical position of window
				, rc.right-rc.left 			// window width
				, rc.bottom-rc.top			// window height
				, NULL                    // handle to parent or owner window
				, NULL                    // handle to menu, or child-window identifier
				, hInstance               // handle to application instance
				, 0				       // pointer to window-creation data
			);
		}

		EnableOpenGL( m_hWnd, &m_hDC, &m_hGLRC );

		ShowWindow ( m_hWnd, SW_NORMAL );			// WM_PAINT


	}
}

//------------------------------------------------------------------------------
void	Win::SetFuncPaint( void (*func)() )
//------------------------------------------------------------------------------
{
	m_pFuncPaint = func;
}

//------------------------------------------------------------------------------
void	 Win::Swap()
//------------------------------------------------------------------------------
{
	SwapBuffers( m_hDC );

}

//------------------------------------------------------------------------------
void	 Win::run()
//------------------------------------------------------------------------------
{
	int	flgLoop = false;
	do
	{
		while (true)
		{
			MSG	msg;
			if (PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE) )
			{
				if (msg.message == WM_QUIT)
				{
					flgLoop = false;
					break;
				}
				else
				{
					DispatchMessage(&msg); // WM_581
				}
			}
			else
			{
				if ( m_pFuncPaint ) m_pFuncPaint();
				Swap();

				flgLoop = true;
				break;
			}
		}

	}
	while( flgLoop );

}
} win;


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
	case WM_SYNCPAINT:	printf( "WM_SYNCPAINT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETFONT:	printf( "WM_SETFONT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETHOTKEY:	printf( "WM_SETHOTKEY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETICON:	printf( "WM_SETICON(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETREDRAW:	printf( "WM_SETREDRAW(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETTEXT:	printf( "WM_SETTEXT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_SETTINGCHANGE:	printf( "WM_SETTINGCHANGE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;

	case WM_SIZE:								// Resize The OpenGL Window
		{
			win.w = LOWORD(lParam);
			win.h = HIWORD(lParam);
			glViewport(0,0, win.w, win.h );
			if ( win.m_pFuncPaint ) win.m_pFuncPaint();
			win.Swap();
		}
		break;
		
//	case WM_PAINT:	printf( "WM_PAINT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );		break;
//	case WM_NCDESTROY:	printf( "WM_NCDESTROY(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_GETICON:	printf( "WM_GETICON(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCCREATE:	printf( "WM_NCCREATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_PAINT:				printf( "WM_PAINT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
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
//	case WM_EXITSIZEMOVE:		printf( "WM_EXITSIZEMOVE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_EXITSIZEMOVE:
//		DisableOpenGL( hWnd, m_hDC, m_hGLRC );
//		EnableOpenGL( hWnd, &m_hDC, &m_hGLRC );
		break;
//	case WM_WINDOWPOSCHANGING:	printf( "WM_WINDOWPOSCHANGING(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_WINDOWPOSCHANGED:	printf( "WM_WINDOWPOSCHANGED(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ERASEBKGND:			printf( "WM_ERASEBKGND(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCPAINT:			printf( "WM_NCPAINT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MOUSEACTIVATE:		printf( "WM_MOUSEACTIVATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ACTIVATE:			printf( "WM_ACTIVATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_SETFOCUS:			printf( "WM_SETFOCUS(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_KILLFOCUS:			printf( "WM_KILLFOCUS(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_NCACTIVATE:			printf( "WM_NCACTIVATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_GETTEXT:			printf( "WM_GETTEXT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MDIACTIVATE:		printf( "WM_MDIACTIVATE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ACTIVATEAPP:		printf( "WM_ACTIVATEAPP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
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
//	case WM_SYSKEYUP:			printf( "WM_SYSKEYUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_SYSKEYDOWN:			printf( "WM_SYSKEYDOWN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ENTERMENULOOP:		printf( "WM_ENTERMENULOOP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_INITMENU:			printf( "WM_INITMENU(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_MENUSELECT:			printf( "WM_MENUSELECT(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_ENTERIDLE:			printf( "WM_ENTERIDLE(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_CHAR:				printf( "WM_CHAR(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
//	case WM_KEYUP:				printf( "WM_KEYUP(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );break;
	case WM_KEYDOWN:			/*printf( "WM_KEYDOWN(wp=%d,lp=%d,%d)\n",wParam,HIWORD(lParam),LOWORD(lParam) );*/
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
//		printf("WM_DESTROY> %d\n", __LINE__ );
		PostQuitMessage( 0 );
		break;
	default:
//		printf("Error Invalid Message: (%d) : %s(%d) in %s\n", uMsg, __func__, __LINE__, __func__ );
//		exit(1);
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
		WM_581
		WM_581
		WM_DESTROY
		WM_NCDESTROY
*/


	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

class Framework
{
public:
	virtual void	Create() = 0;
	virtual void	Paint() = 0;
	virtual void	Remove() = 0;
};

//------------------------------------------------------------------------------
void	Paint()
//------------------------------------------------------------------------------
{



		static	ModelSphere	sphere;

		static	ModelTex	cube1;
		static	ModelTex	cube2;

		mat_identity( sphere.M );
		mat_scaling( sphere.M, vec3( 1.0, 1.0, 1.0 ) );
		mat_translate( sphere.M, vec3(  0.0, 0.0 , 0.0 ) );

		mat_identity( cube1.M );
		mat_scaling( cube1.M, vec3( 0.3, 1.0 , 0.3 ) );
		mat_translate( cube1.M, vec3( 1.2, -0.2 , 0 ) );

		mat_identity( cube2.M );
		mat_scaling( cube2.M, vec3(  0.3, 1.0 , 0.3 ) );
		mat_translate( cube2.M, vec3( -1.2, -0.2 , 0 ) );



		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sphere.DrawSphere();

		cube1.DrawTex();
		cube2.DrawTex();



}

//------------------------------------------------------------------------------
int main()
//------------------------------------------------------------------------------
{

	win.SetFuncPaint( Paint );

	win.run();

	return  0;
}



