#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <conio.h>
#include <string>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

GLuint texture = 0;
BITMAP BMP;
HBITMAP hBMP = NULL;

float xRotateDeg = 0, yRotateDeg = 0, zRotateDeg = 0;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 0x58:
			xRotateDeg += 1.5f;
			break;
		case 0x59:
			yRotateDeg += 1.5f;
			break;
		case 0x5A:
			zRotateDeg += 1.5f;
			break;
		case VK_SPACE:
			xRotateDeg = 0;
			yRotateDeg = 0;
			zRotateDeg = 0;
			break;
		}
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
	glRotatef(xRotateDeg, 1, 0, 0);
	glRotatef(yRotateDeg, 0, 1, 0);
	glRotatef(zRotateDeg, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glTexCoord3f(0.5, 1, 0); glVertex3f(0.5, 1, 0);
	glTexCoord3f(0, 0, 0);   glVertex3f(0, 0, 0.5);
	glTexCoord3f(1, 0, 0);   glVertex3f(1, 0, 0.5);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(xRotateDeg, 1, 0, 0);
	glRotatef(yRotateDeg, 0, 1, 0);
	glRotatef(zRotateDeg, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glTexCoord3f(0.5, 1, 0); glVertex3f(0.5, 1, 0);
	glTexCoord3f(0, 0, 0);   glVertex3f(0, 0, 0.5);
	glTexCoord3f(1, 0, 0);   glVertex3f(0, 0, -0.5);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(xRotateDeg, 1, 0, 0);
	glRotatef(yRotateDeg, 0, 1, 0);
	glRotatef(zRotateDeg, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glTexCoord3f(0.5, 1, 0); glVertex3f(0.5, 1, 0);
	glTexCoord3f(0, 0, 0);   glVertex3f(1, 0, 0.5);
	glTexCoord3f(1, 0, 0);   glVertex3f(1, 0, -0.5);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(xRotateDeg, 1, 0, 0);
	glRotatef(yRotateDeg, 0, 1, 0);
	glRotatef(zRotateDeg, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glTexCoord3f(0.5, 1, 0); glVertex3f(0.5, 1, 0);
	glTexCoord3f(0, 0, 0);   glVertex3f(1, 0, -0.5);
	glTexCoord3f(1, 0, 0);   glVertex3f(0, 0, -0.5);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(xRotateDeg, 1, 0, 0);
	glRotatef(yRotateDeg, 0, 1, 0);
	glRotatef(zRotateDeg, 0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord3f(0, 1, 0); glVertex3f(0, 0, 0.5);
	glTexCoord3f(1, 1, 0); glVertex3f(1, 0, 0.5);
	glTexCoord3f(1, 0, 0); glVertex3f(1, 0, -0.5);
	glTexCoord3f(0, 0, 0); glVertex3f(0, 0, -0.5);
	glEnd();
	glPopMatrix();
}

void initTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		"res/BrickFacade0003_1_seamless_S.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION |
		LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
		BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
}

void display()
{
	glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	initTexture();
	myPyramid();
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