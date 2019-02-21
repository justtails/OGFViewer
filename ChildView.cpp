// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "OGFViewer.h"
#include "ChildView.h"
#include "OGFLoader.h"
#include "glee.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
float z = 0;

CChildView::CChildView()
{
	m_Model = NULL;
	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( "OGFViewer.cfg", CFile::modeRead, &fileException ) )
	{
		TRACE( "Can't open file %s, error = %u\n",
			"OGFViewer.cfg", fileException.m_cause );
	}
	char      szBuffer[256];
	memset(szBuffer,0,256);
	UINT nActual = myFile.Read( szBuffer, sizeof( szBuffer ) );
	m_PathToStalker.assign(szBuffer);
	m_MouseLPressed = false;
	m_MouseRPressed = false;
}

CChildView::~CChildView()
{
}

BOOL CChildView::InitRender()
{
	if(!m_hWnd)
		return FALSE;
	m_DContext = ::GetDC( m_hWnd );
	PIXELFORMATDESCRIPTOR pfd =
	{ 
		sizeof ( PIXELFORMATDESCRIPTOR ),   // size of this pfd 
			1,                     // version number 
			PFD_DRAW_TO_WINDOW |   // support window 
			PFD_SUPPORT_OPENGL |   // support OpenGL 
			PFD_DOUBLEBUFFER |       // double buffered 
			PFD_GENERIC_ACCELERATED, // accelerated
			PFD_TYPE_RGBA,         // RGBA type 
			(BYTE)32,                    // color depth 
			0, 0, 0, 0, 0, 0,      // color bits ignored 
			0,                     // no alpha buffer 
			0,                     // shift bit ignored 
			0,                     // no accumulation buffer 
			0, 0, 0, 0,            // accum bits ignored 
			(BYTE)32,                    // z-buffer 
			0,                     // stencil buffer 
			0,                     // no auxiliary buffer 
			PFD_MAIN_PLANE,        // main layer     0,                     // reserved 
			0, 0, 0                // layer masks ignored
	}; 


	int iPixelFormat = ChoosePixelFormat( m_DContext, &pfd ); 
	if ( !iPixelFormat )
	{
		return FALSE;
	}

	if ( pfd.cDepthBits == 0 )
	{
		return FALSE;
	}

	if ( !SetPixelFormat( m_DContext, iPixelFormat, &pfd ))
	{
		return FALSE;
	}


	m_RContext = wglCreateContext( m_DContext );
	if ( !m_RContext )
	{
		DeleteDC( m_DContext );
		m_DContext = 0;
		return FALSE;
	}

	if ( !wglMakeCurrent( m_DContext, m_RContext ) )
	{
		DeleteDC(m_DContext);
		m_DContext = 0;
		wglDeleteContext(m_RContext);
		m_RContext = 0;
		return FALSE;
	}

	//glFrontFace(GL_CW);	
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);	
	glPolygonMode( GL_FRONT, GL_FILL );
	glPolygonMode( GL_BACK, GL_FILL );
	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glClearColor( 0,0,0,0 );
	glEnable(GL_DEPTH_TEST);
	return TRUE;
}

void CChildView::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_Model)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		int s = m_OldMouseSPos.y - m_NewMouseSPos.y;

		static float tran = 2.0f;
		if(m_MouseRPressed)
			tran += (float)(s / 50.0f);
		if(tran > 10)
		{
			tran = 10;
		}
		if(tran < 0)
		{
			tran =0;
		}
		glTranslatef(0,0,-tran);
		int x = m_OldMousePos.x - m_NewMousePos.x;
		int y = m_OldMousePos.y - m_NewMousePos.y;
		
		static anglex = 0;
		static angley = 0;
		anglex += x;
		angley += y;
		glRotatef(270,1,0,0);
		glRotatef(anglex,0,0,1);
		glRotatef(angley,1,0,0);
		m_Model->Render();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		m_OldMousePos = m_NewMousePos;
		m_OldMouseSPos = m_NewMouseSPos;
	}
	glFinish();
	SwapBuffers(m_DContext);
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_OPEN, LoadModel)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN( )
	ON_WM_LBUTTONUP( )
	ON_WM_RBUTTONDOWN( )
	ON_WM_RBUTTONUP( )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView drawing

void CChildView::OnDraw(CDC* pDC)
{
	RenderScene();
}

void CChildView::OnMouseMove(UINT nFlags,CPoint point)
{
	if(m_MouseLPressed)
	{
		m_NewMousePos = point;
	}
	if(m_MouseRPressed)
	{
		m_NewMouseSPos = point;
	}
	RenderScene();
}

 void CChildView::OnLButtonDown(UINT nFlags,CPoint point)
 {
	 m_MouseLPressed = true;
	 m_OldMousePos = point;
 }

  void CChildView::OnLButtonUp(UINT nFlags,CPoint point)
 {
	 m_MouseLPressed = false;
 }

 void CChildView::OnRButtonDown(UINT nFlags,CPoint point)
 {
	 m_MouseRPressed = true;
	 m_OldMouseSPos = point;
 }

  void CChildView::OnRButtonUp(UINT nFlags,CPoint point)
 {
	 m_MouseRPressed = false;
 }
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	hInst = cs.hInstance;
	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ( !InitRender() )
	{
		MessageBox( "Error setting up render system!", "Init Error!",
			MB_OK | MB_ICONERROR );
		return -1;
	}
	return 0;
}

// Shutdown this view when window is destroyed.
void CChildView::OnDestroy()
{
	CWnd::OnDestroy();
}

// Override the errase background function to
// do nothing to prevent flashing.
BOOL CChildView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CChildView::OnSize(UINT nType,int cx,int cy)
{
	CWnd::OnSize(nType, cx, cy);
	glViewport( 0, 0, cx, cy );
	if(cx != 0 || cy != 0)
	{
		glMatrixMode( GL_PROJECTION ); 
		glLoadIdentity(); 
		gluPerspective(45,cx / cy,0.1,1000);
		glMatrixMode( GL_MODELVIEW );
	}

	m_WindowWidth = cx;
	m_WindowHeight = cy;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CWnd::OnPaint() for painting messages
	RenderScene();
}

void CChildView::LoadModel()
{
	OPENFILENAME ofn;
	char szCurrentDirectory[_MAX_PATH];
	char szFile[_MAX_PATH] ="";
	HRESULT hr = E_FAIL;

	GetCurrentDirectory(_MAX_PATH, szCurrentDirectory);

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT;

	ofn.hwndOwner	= m_hWnd;
	ofn.lpstrFile	= szFile;
	ofn.lpstrTitle	= "Load OGF file";
	ofn.lpstrFilter = "OGF Files (*.OGF)\0*.OGF\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrDefExt = "OGF";

	// Ask for filename
	if(GetOpenFileName(&ofn))
	{
		pszFileName = (LPCSTR)ofn.lpstrFile;
		hr = S_OK;
	}

	SetCurrentDirectory(szCurrentDirectory);
	if(m_Model)
	{
		delete m_Model;
		m_Model = NULL;
	}
	m_Model = Load((char*)pszFileName,m_PathToStalker);
}
