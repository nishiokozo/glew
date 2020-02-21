#include 	"sys.h"


//------------------------------------------------------------------------------
Hdr::Hdr( int width, int height, int type, bool nearest_linear )
//------------------------------------------------------------------------------
{
	memset ( this, 0, sizeof(*this) );

	m_pFbo1 = new Fbo( width, height, type, nearest_linear );
	m_pFbo2 = new Fbo( width, height, type, nearest_linear );

	m_pStreach_hdr		= new Filter( "f_streach_hdr.glsl" );
	m_pStreach_blend	= new Filter( "f_streach_blend.glsl" );
	m_pGaussianv		= new Filter( "f_gaussian_v.glsl" );
	m_pGaussianh		= new Filter( "f_gaussian_h.glsl" );
}

//------------------------------------------------------------------------------
void	Hdr::DrawDirect( GLuint hdlTex )
//------------------------------------------------------------------------------
{
		m_pStreach_blend->Draw_blend( hdlTex, 0 );

}
//------------------------------------------------------------------------------
void	Hdr::DrawBloom( GLuint hdlTex, int lenWeight, double s )
//------------------------------------------------------------------------------
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_pFbo1->m_hdlFbo );
	glViewport(0,0, m_pFbo1->m_w, m_pFbo1->m_h );
	glClear( GL_COLOR_BUFFER_BIT );


	m_pStreach_hdr->DrawTexture( hdlTex );


	if ( sys.key_Toggle( 'B' ) )
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0, sys.m_w, sys.m_h );
		glClear( GL_COLOR_BUFFER_BIT );
		m_pStreach_blend->Draw_blend( hdlTex, 0 );
		return;
	}

	if ( !sys.key_Toggle( 'G' ) )
	{
		float	tblWeight[ lenWeight ];
	
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

		glBindFramebuffer(GL_FRAMEBUFFER, m_pFbo2->m_hdlFbo );
		glViewport(0,0, m_pFbo2->m_w, m_pFbo2->m_h );
		m_pGaussianv->DrawGaussian( m_pFbo1->m_color_tex, 1.0f/m_pFbo1->m_h, lenWeight, tblWeight );

		glBindFramebuffer(GL_FRAMEBUFFER, m_pFbo1->m_hdlFbo );
		glViewport(0,0, m_pFbo1->m_w, m_pFbo1->m_h );
		m_pGaussianh->DrawGaussian( m_pFbo2->m_color_tex, 1.0f/m_pFbo2->m_w, lenWeight, tblWeight );

	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0, sys.m_w, sys.m_h );
	glClear( GL_COLOR_BUFFER_BIT );
	glClearColor( 0.0f, 0.0f, 1.0f, 0.0f );

	if ( !sys.key_Toggle( VK_TAB ) )
	{
		m_pStreach_blend->Draw_blend( hdlTex, m_pFbo1->m_color_tex );
	}
	else
	{
		m_pStreach_blend->Draw_blend( m_pFbo1->m_color_tex, 0);
	}
//		m_pStreach_blend->Draw_blend( hdlTex.GetTexture(), 0);

}
