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

void initTexture(int pointer)
{
	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &texture);

	LPCSTR dir = "";
	switch (pointer)
	{
	case 0: // Cone
		dir = "res/ice-cream-pattern-wafle.bmp";
		break;
	case 1: // Chocolate
		dir = "res/chocolate-ice-cream.bmp";
		break;
	case 2: // Vanilla
		dir = "res/vanilla-ice-cream.bmp";
		break;
	case 3: // Cherry
		dir = "res/cherry-red.bmp";
		break;
	case 4: // Chip
		dir = "res/waffle-side-short-horizontal.bmp";
		break;
	case 5: // Waffle Side
		dir = "res/waffle-side-short-verticle.bmp";
		break;
	case 6: // Waffle Topping
		dir = "res/ice-cream-pattern-wafle-choc.bmp";
		break;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		dir, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION |
		LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
		BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);
}

void myCone()
{
	initTexture(0);

	GLUquadricObj* var = NULL;
	var = gluNewQuadric();

	glPushMatrix();
	glColor3ub(171, 124, 44);
	glRotatef(70, 1, 0, 0);
	gluQuadricDrawStyle(var, GLU_FILL);
	gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0.25f, 0.005f, 0.9f, 32, 32);
	glPopMatrix();
}

void myIceCream()
{
	GLUquadricObj* var = NULL;
	var = gluNewQuadric();

	initTexture(1);
	glPushMatrix();
	glColor3ub(180, 255, 145);
	glRotatef(70, 1, 0, 0);
	gluQuadricDrawStyle(var, GLU_FILL);
	gluQuadricTexture(var, GL_TRUE);
	gluSphere(var, 0.235f, 32, 32);
	glPopMatrix();

	initTexture(2);
	glPushMatrix();
	glColor3ub(227, 227, 227);
	glTranslatef(0, 0.25f, -0.05f);
	glRotatef(70, 1, 0, 0);
	gluQuadricDrawStyle(var, GLU_FILL);
	gluQuadricTexture(var, GL_TRUE);
	gluSphere(var, 0.22f, 32, 32);
	glPopMatrix();
}

void myCherry()
{
	GLUquadricObj* var = NULL;
	var = gluNewQuadric();

	initTexture(3);
	glPushMatrix();
	glColor3ub(255, 31, 31);
	glTranslatef(0.15f, 0.4f, -0.15f);
	glRotatef(70, 1, 0, 0);
	gluQuadricDrawStyle(var, GLU_FILL);
	gluQuadricTexture(var, GL_TRUE);
	gluSphere(var, 0.05f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15f, 0.4f, -0.15f);
	glRotatef(70, 1, 1, 0);
	glBegin(GL_LINE_STRIP);
	glColor3ub(255, 31, 31);
	glVertex3f(0.1f, 0.15f, 0);
	glVertex3f(0, 0.15f, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();
}

void myWaffle()
{
	initTexture(4);
	glPushMatrix();
	glTranslatef(0, 0.35f, -0.2f);
	glRotatef(-60, 1, 0, 1);
	glBegin(GL_QUADS);

	// Side Top
	glTexCoord2f(0, 1); glVertex3f(-0.05f, 0.025f, 0);
	glTexCoord2f(1, 1); glVertex3f(0.05f, 0.025f, 0);
	glTexCoord2f(1, 0); glVertex3f(0.05f, -0.025f, 0);
	glTexCoord2f(0, 0); glVertex3f(-0.05f, -0.025f, 0);
	// Side Bottom
	glTexCoord2f(0, 1); glVertex3f(0.05f, -0.025f, 0.55f);
	glTexCoord2f(1, 1); glVertex3f(0.05f, 0.025f, 0.55f);
	glTexCoord2f(1, 0); glVertex3f(-0.05f, 0.025f, 0.55f);
	glTexCoord2f(0, 0); glVertex3f(-0.05f, -0.025f, 0.55f);

	glEnd();
	glPopMatrix();


	initTexture(5);
	glPushMatrix();
	glTranslatef(0, 0.35f, -0.2f);
	glRotatef(-60, 1, 0, 1);
	glBegin(GL_QUADS);

	// Side +X
	glTexCoord2f(0, 1); glVertex3f(0.05f, 0.025f, 0);
	glTexCoord2f(1, 1); glVertex3f(0.05f, -0.025f, 0);
	glTexCoord2f(1, 0); glVertex3f(0.05f, -0.025f, 0.55f);
	glTexCoord2f(0, 0); glVertex3f(0.05f, 0.025f, 0.55f);
	// Side -X
	glTexCoord2f(0, 1); glVertex3f(-0.05f, -0.025f, 0.55f);
	glTexCoord2f(1, 1); glVertex3f(-0.05f, 0.025f, 0.55f);
	glTexCoord2f(1, 0); glVertex3f(-0.05f, 0.025f, 0);
	glTexCoord2f(0, 0); glVertex3f(-0.05f, -0.025f, 0);

	glEnd();
	glPopMatrix();


	initTexture(6);
	glPushMatrix();
	glTranslatef(0, 0.35f, -0.2f);
	glRotatef(-60, 1, 0, 1);
	glBegin(GL_QUADS);

	// Top
	glTexCoord2f(0, 1); glVertex3f(0.05f, 0.025f, 0.55f);
	glTexCoord2f(1, 1); glVertex3f(-0.05f, 0.025f, 0.55f);
	glTexCoord2f(1, 0); glVertex3f(-0.05f, 0.025f, 0);
	glTexCoord2f(0, 0); glVertex3f(0.05f, 0.025f, 0);
	// Bottom
	glTexCoord2f(0, 1); glVertex3f(0.05f, -0.025f, 0.55f);
	glTexCoord2f(1, 1); glVertex3f(-0.05f, -0.025f, 0.55f);
	glTexCoord2f(1, 0); glVertex3f(-0.05f, -0.025f, 0);
	glTexCoord2f(0, 0); glVertex3f(0.05f, -0.025f, 0);

	glEnd();
	glPopMatrix();
}

void myChip()
{
	GLUquadricObj* var = NULL;
	var = gluNewQuadric();

	initTexture(4);
	glPushMatrix();
	glColor3ub(255, 247, 135);
	glTranslatef(-0.15f, 0.45f, -0.13f);
	glRotatef(90, 0, 1, 1);
	gluQuadricDrawStyle(var, GLU_FILL);
	gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0.069f, 0.07f, 0.1f, 32, 32);
	glPopMatrix();
}

void display()
{
	glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	myCone();
	myIceCream();
	myCherry();
	myChip();
	myWaffle();
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
