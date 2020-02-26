#include <windows.h>
#include "win.h"


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
		case WM_CREATE:
				break;

		case WM_KEYDOWN:
			if ( wParam == VK_ESCAPE ) SendMessage(hWnd , WM_CLOSE , 0 , 0);	
			return 0;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//------------------------------------------------------------------------------
Win::Win( int width, int height )
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



		ShowWindow (m_hWnd, SW_NORMAL);
	}
}

//------------------------------------------------------------------------------
Win::~Win()
//------------------------------------------------------------------------------
{

	DestroyWindow( m_hWnd );
}
//------------------------------------------------------------------------------
boolean	 Win::update()
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

