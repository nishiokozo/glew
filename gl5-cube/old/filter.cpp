#include "sys.h"

//------------------------------------------------------------------------------
Filter::~Filter()
//------------------------------------------------------------------------------
{
}
//------------------------------------------------------------------------------
Filter::Filter( const char* fnShader )
//------------------------------------------------------------------------------
{
	float	tblVertex[] =
	{
		-1.0f, -1.0f,	0.0f,
		 1.0f, -1.0f,	0.0f,
		 1.0f,  1.0f,	0.0f,
		-1.0f,  1.0f,	0.0f,
	};
	sys.gl_CreateBuffer( &m_hdlVertex_Position, GL_ARRAY_BUFFER, sizeof(tblVertex), tblVertex );
	int	tblIndex[] =	{		0,1,2,		2,3,0,	};
	sys.gl_CreateBuffer( &m_hdlVertex_Index, GL_ELEMENT_ARRAY_BUFFER, sizeof(tblIndex), tblIndex );
	m_cntIndex = sizeof(tblIndex)/sizeof(int);
	sys.shader_LoadFile( &m_hdlProgram, fnShader );
	m_hdl_Position		= glGetAttribLocation( m_hdlProgram, "Position");
	m_hdl_Tex0		= glGetUniformLocation( m_hdlProgram, "Tex0");

	// for gaussian
	m_hdl_Length		= glGetUniformLocation( m_hdlProgram, "Length");
	m_hdl_Weight		= glGetUniformLocation( m_hdlProgram, "Weight");
	m_hdl_Offset		= glGetUniformLocation( m_hdlProgram, "ofset");

	//	for	blend
	m_hdl_Tex1		= glGetUniformLocation( m_hdlProgram, "Tex1");
}

//------------------------------------------------------------------------------
void Filter::ReloadShader( const char* fnShader )
//------------------------------------------------------------------------------
{

	sys.shader_Remove( m_hdlProgram ) ;
	sys.shader_LoadFile( &m_hdlProgram, fnShader ) ;

				printf("Reload %s %d\n", fnShader, m_hdlProgram );
}
//------------------------------------------------------------------------------
void Filter::DrawTexture( int hdlTex )
//------------------------------------------------------------------------------
{
	glUseProgram( m_hdlProgram );

	sys.gl_SetBuffer( m_hdl_Position, m_hdlVertex_Position, GL_ARRAY_BUFFER, 3, GL_FLOAT );
	sys.gl_SetTexture( GL_TEXTURE_2D, m_hdl_Tex0, 0, hdlTex );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glDrawElements( GL_TRIANGLE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );
}
//------------------------------------------------------------------------------
void Filter::DrawFilter()
//------------------------------------------------------------------------------
{
	glUseProgram( m_hdlProgram );

	sys.gl_SetBuffer( m_hdl_Position, m_hdlVertex_Position, GL_ARRAY_BUFFER, 3, GL_FLOAT );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glDrawElements( GL_TRIANGLE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );
}

//------------------------------------------------------------------------------
void Filter::Draw_blend( int tex0, int tex1 )
//------------------------------------------------------------------------------
{
	glUseProgram( m_hdlProgram );

	sys.gl_SetBuffer( m_hdl_Position, m_hdlVertex_Position, GL_ARRAY_BUFFER, 3, GL_FLOAT );
	sys.gl_SetTexture( GL_TEXTURE_2D, m_hdl_Tex0, 0, tex0 );
	sys.gl_SetTexture( GL_TEXTURE_2D, m_hdl_Tex1, 1, tex1 );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glDrawElements( GL_TRIANGLE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );
}

//------------------------------------------------------------------------------
void Filter::DrawGaussian( int hdlTex, float pitch, int lenWeight, float* tblWeight )
//------------------------------------------------------------------------------
{
	glUseProgram( m_hdlProgram );

	sys.gl_SetBuffer( m_hdl_Position, m_hdlVertex_Position, GL_ARRAY_BUFFER, 3, GL_FLOAT );
	sys.gl_SetTexture( GL_TEXTURE_2D, m_hdl_Tex0, 0, hdlTex );


	glUniform1f(m_hdl_Offset, pitch );
	glUniform1i(m_hdl_Length, 10 );
	glUniform1fv(m_hdl_Weight, lenWeight, tblWeight );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glDrawElements( GL_TRIANGLE_STRIP, m_cntIndex, GL_UNSIGNED_INT, 0 );

}
