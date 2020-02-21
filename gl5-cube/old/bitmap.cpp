#include "sys.h"

//------------------------------------------------------------------------------
Bitmap::~Bitmap()
//------------------------------------------------------------------------------
{
	free( m_pBuf );
	m_pBuf = 0;
}

//------------------------------------------------------------------------------
Bitmap::Bitmap( int width, int height, int type, bool nearest_linear )
//------------------------------------------------------------------------------
{
	memset( this, 0, sizeof(this) );

	m_type = type;

	{
		m_width = width;
		m_height = height;

		m_pBuf = (float*)malloc( 4*m_width*m_height*sizeof(float) );

		int	min = nearest_linear?GL_LINEAR:GL_NEAREST;
		int	mag = nearest_linear?GL_LINEAR:GL_NEAREST;
		
		sys.gl_CreateTexture( &m_hdl_idTexture, GL_TEXTURE_2D, GL_RGBA32F, m_width, m_height, min, mag, GL_REPEAT );

	}

}

//------------------------------------------------------------------------------
void Bitmap::PutColor( int x, int y, float r, float g, float b )
//------------------------------------------------------------------------------
{
	float	(*ptr)[m_width][4] = (float(*)[m_width][4])m_pBuf;
	ptr[y][x][0] = r;
	ptr[y][x][1] = g;
	ptr[y][x][2] = b;
	ptr[y][x][3] = 1.0;

}

//------------------------------------------------------------------------------
void Bitmap::UpdateTexture()
//------------------------------------------------------------------------------
{
	sys.gl_LoadTexture( GL_TEXTURE_2D, m_hdl_idTexture, GL_RGBA32F, m_width, m_height, GL_RGBA, GL_FLOAT, m_pBuf );
}

//------------------------------------------------------------------------------
GLuint Bitmap::GetTexture()
//------------------------------------------------------------------------------
{
	return m_hdl_idTexture;
}
