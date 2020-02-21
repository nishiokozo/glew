#include "sys.h"
#include "vec.h"
#include "shape.h"
#include "shader.h"
#include "loaderbmp.h"

//------------------------------------------------------------------------------
Shader::Shader( const char* fn )
//------------------------------------------------------------------------------
{
	sys.shader_LoadFile( &m_hdlProgram, fn );

	m_hdl_buf_Position			= glGetAttribLocation( m_hdlProgram, "Position");
	m_hdl_Uv					= glGetAttribLocation( m_hdlProgram, "Uv");
	m_hdl_bufer_Normal			= glGetAttribLocation( m_hdlProgram, "Normal");
	m_hdl_var_Projection		= glGetUniformLocation( m_hdlProgram, "Projection");
	m_hdl_var_Model				= glGetUniformLocation( m_hdlProgram, "matModel");
	m_hdl_var_ColModel			= glGetUniformLocation( m_hdlProgram, "ColModel");

//printf(" shd %d %d\n", m_hdl_buf_Position, m_hdl_bufer_Normal );



if (0)
	{
	LoaderBmp	pic( "dra.bmp" );
printf("pic %d %d\n", pic.width(), pic.height() );

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
void Shader::DrawPN( const Shape& shape, const mat4& matModel, const mat4& matProj )
//------------------------------------------------------------------------------
{

	glUseProgram( (*this).m_hdlProgram );


	glUniform3f( (*this).m_hdl_var_ColModel, 1,1,0 );


	glUniformMatrix4fv( (*this).m_hdl_var_Model,      1, GL_TRUE, matModel.m_array );
	glUniformMatrix4fv( (*this).m_hdl_var_Projection, 1, GL_TRUE, matProj.m_array );

	glEnableVertexAttribArray( (*this).m_hdl_buf_Position );
	glBindBuffer(GL_ARRAY_BUFFER, shape.m_hdlVertex_Position  );
	glVertexAttribPointer( (*this).m_hdl_buf_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray( (*this).m_hdl_bufer_Normal );
	glBindBuffer(GL_ARRAY_BUFFER, shape.m_hdlVertex_Normal  );
	glVertexAttribPointer( (*this).m_hdl_bufer_Normal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape.m_hdlVertex_Index );
	glDrawElements( GL_TRIANGLE_STRIP, shape.m_cntIndex, GL_UNSIGNED_INT, 0 );

	glUseProgram( 0 );

}

