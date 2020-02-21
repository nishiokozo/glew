#include	<iostream>
using namespace std;
#include <string>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
//  #include <AL/al.h>
//  #include <AL/alut.h>

#define	_line	printf("--%s(%d)\n",__FILE__,__LINE__)



//class IProc
//{
//public:
//	virtual	 	~IProc() {}
//
//	virtual void	Paint() = 0;
//};



class Sys
{
	void	 Sync();
//	void	SysRemove();
//	void	SysCreate();
//	void	SysSetProc( IProc& fw );
//	void	SysSwap();
//	void	SysRun();

public:
	~Sys();
	Sys( const char* fn );

	boolean	m_bError;
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hGLRC;
	LARGE_INTEGER m_frequency; 
	double	m_timerFrammesuree;
	double	m_timerSec;
	double	m_timerFrame;


//	IProc*	m_pProc;

	int	m_w;
	int	m_h;

	double	GetSecond();
	bool	 Update( double timeSync );

	bool	file_malloc_Load( const char* fname, char** ppBuf, int* pLen );

	void	EnableOpenGL(HWND hWnd, HDC * pHDC, HGLRC * hGLRC);
	void	DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hGLRC);
	bool	printShaderInfoLog(	GLuint shader );

	char*	m_env_pBuf; 
	int		m_env_len;
	char	m_env_str[256];

	bool	env_FindValue( const char* name, char* str, int len );
	float	env_GetFloat( const char* name );
	int		env_GetInt( const char* name );
	string	env_GetString( const char* name );

	bool	shader_Compile(		int		hdlProgram,		int		type,		const char*	fn	);
	void 	shader_LoadFile( GLuint*	pHdlProgram, const char* fn );
	void	shader_Remove( GLuint hdlProgram );

	void	gl_CreateBuffer( GLuint* pHdl, int type, int size, void* tbl );
	void	gl_SetBuffer( GLuint hdlTag, GLuint hdlBuffer, int type, int sizeVal, int defVal );
	void	gl_SetBuffer( GLint hdlTag, GLuint hdlBuffer, int type, int sizeVal, int defVal );
	void	gl_SetTexture( int target, int hdl_Texture, int num, GLuint hdl_idTexture );
	void	gl_CreateTexture( GLuint*  p_hdl_Texture, int target, int format, int width, int height, int min, int mag, int warp );
	void	gl_LoadTexture( int target, GLuint hdl_Texture, int innerformat, int width, int height, int format, int type, void* pBuf );

//	Bitmap* bitmap_Create( const char* fnShader, int width, int height );
//	bool	 Update();

	bool	key_Press( int val );
	bool	key_Toggle( int val );
	bool	key_Trigger( int val );


	double	m_measure_timePerFrame_ms;
	bool	m_measure_flgInfomation;
	bool	measure_GetTimerPaint();

};


extern	class Sys	sys;


