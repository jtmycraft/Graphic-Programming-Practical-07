#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <conio.h>
#include <string>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

GLfloat Va[] = { 0.4,0.4,0.4,1 };           //光源环境光强度数组  
GLfloat Vd[] = { 0.6,0.6,0.6,1 };           //光源散射光强度数组  
GLfloat Vs[] = { 0.6,0.6,0.6,1 };           //光源镜面反射光强度数组  
GLfloat Vp[] = { 1,1,1,1 };             //光源位置数组

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CHAR:
		break;

	case WM_KEYDOWN:
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void myPyramid()
{
	glPushMatrix();
	glColor3ub(180, 255, 145);
	glTranslatef(0, -0.5f, 0);
	glBegin(GL_POLYGON);

	glColor3f(0, 1, 0);
	glVertex3f(0, 1, -0.5);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(0.5, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 1, -0.5);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(-0.5, 0, -1.0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 1, -0.5);
	glVertex3f(0.5, 0, 0);
	glVertex3f(0.5, 0, -1.0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 1, -0.5);
	glVertex3f(0.5, 0, -1.0);
	glVertex3f(-0.5, 0, -1.0);

	glColor3f(0, 1, 0);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(0.5, 0, 0);
	glVertex3f(0.5, 0, -1.0);
	glVertex3f(-0.5, 0, -1.0);

	glEnd();
	glPopMatrix();
}

void mySphere()
{
	GLUquadricObj* var = NULL;
	var = gluNewQuadric();

	glPushMatrix();
	glColor3ub(180, 255, 145);
	//gluQuadricDrawStyle(var, GLU_LINE);
	gluSphere(var, 0.5f, 32, 32);
	glPopMatrix();
}

void display()
{
	glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


}

//--------------------------------------------------------------------

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
	_In_ LPSTR cmdLine, _In_ int nCmdShow) // FIXED
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------