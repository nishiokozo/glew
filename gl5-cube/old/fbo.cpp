#include	"sys.h"

//-----------------------------------------------------------------------------
Fbo::Fbo( int w, int h, int type, bool nearest_linear )
//-----------------------------------------------------------------------------
{
	memset( this, 0, sizeof(*this));
	
	m_w = w;
	m_h = h;

	int	min = nearest_linear?GL_LINEAR:GL_NEAREST;
	int	mag = nearest_linear?GL_LINEAR:GL_NEAREST;
	int	wrap = GL_CLAMP_TO_EDGE;

	glGenTextures(1, &m_color_tex);
	glBindTexture(GL_TEXTURE_2D, m_color_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

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
