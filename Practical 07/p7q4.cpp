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
	LPCSTR dir = "";
	switch (pointer)
	{
	case 0: // Cone
		dir = "res/ice-cream-pattern-wafle.bmp";
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

void drawBase(const GLfloat(&trans)[3]) {
	float points[8][3] = {
		{ 0.2f, 0.2f, 0.2f }, // A [0][]
		{ 0.2f, -0.2f, 0.2f }, // B [1][]
		{ -0.2f, -0.2f, 0.2f }, // C [2][]
		{ -0.2f, 0.2f, 0.2f }, // D [3][]
		{ 0.2f, 0.2f, -0.2f }, // E [4][]
		{ 0.2f, -0.2f, -0.2f }, // F [5][]
		{ -0.2f, -0.2f, -0.2f }, // G [6][]
		{ -0.2f, 0.2f, -0.2f }, // H [7][]
	};

	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);
	glBegin(GL_QUADS);

	// Front (ABCD)
	glVertex3f(points[0][0], points[0][1], points[0][2]);
	glVertex3f(points[1][0], points[1][1], points[1][2]);
	glVertex3f(points[2][0], points[2][1], points[2][2]);
	glVertex3f(points[3][0], points[3][1], points[3][2]);
	// Back (EFGH)
	glVertex3f(points[4][0], points[4][1], points[4][2]);
	glVertex3f(points[5][0], points[5][1], points[5][2]);
	glVertex3f(points[6][0], points[6][1], points[6][2]);
	glVertex3f(points[7][0], points[7][1], points[7][2]);

	glEnd();
	glPopMatrix();



	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);
	glBegin(GL_QUADS);

	// Left (CDGH)
	glVertex3f(points[2][0], points[2][1], points[2][2]);
	glVertex3f(points[3][0], points[3][1], points[3][2]);
	glVertex3f(points[6][0], points[6][1], points[6][2]);
	glVertex3f(points[7][0], points[7][1], points[7][2]);
	// Right (ABFE)
	glVertex3f(points[0][0], points[0][1], points[0][2]);
	glVertex3f(points[1][0], points[1][1], points[1][2]);
	glVertex3f(points[5][0], points[5][1], points[5][2]);
	glVertex3f(points[4][0], points[4][1], points[4][2]);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);
	glBegin(GL_QUADS);

	// Top (ADHE)
	glVertex3f(points[0][0], points[0][1], points[0][2]);
	glVertex3f(points[3][0], points[3][1], points[3][2]);
	glVertex3f(points[7][0], points[7][1], points[7][2]);
	glVertex3f(points[4][0], points[4][1], points[4][2]);
	// Bottom (BCGF)
	glVertex3f(points[1][0], points[2][1], points[2][2]);
	glVertex3f(points[2][0], points[3][1], points[3][2]);
	glVertex3f(points[6][0], points[6][1], points[6][2]);
	glVertex3f(points[5][0], points[7][1], points[7][2]);

	glEnd();
	glPopMatrix();
}

void drawPillar(const GLfloat(&trans)[3]) {
	GLUquadricObj* var = NULL;
	var = gluNewQuadric();

	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);
	glRotatef(90, 1, 0, 0);
	gluQuadricDrawStyle(var, GLU_LINE);
	// gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0.02f, 0.02f, 0.8f, 32, 32);
	glPopMatrix();
}

void drawConeRoof(const GLfloat(&trans)[3]) {
	GLUquadricObj* var = NULL;
	var = gluNewQuadric();

	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);
	glRotatef(90, 1, 0, 0);
	gluQuadricDrawStyle(var, GLU_LINE);
	// gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0, 0.03f, 0.2f, 32, 32);
	glPopMatrix();
}

void drawPyramidRoof(const GLfloat(&trans)[3]) {
	float points[8][3] = {
		{ 0.2f, 0, 0.2f }, // A [0][]
		{ -0.2f, 0, 0.2f }, // B [1][]
		{ -0.2f, 0, -0.2f }, // C [2][]
		{ 0.2f, 0, -0.2f }, // D [3][]
		{ 0, 0.2f, 0 }, // E [4][]
	};

	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);
	glBegin(GL_QUADS);

	// Front (ABCD)
	glVertex3f(points[0][0], points[0][1], points[0][2]);
	glVertex3f(points[1][0], points[1][1], points[1][2]);
	glVertex3f(points[2][0], points[2][1], points[2][2]);
	glVertex3f(points[3][0], points[3][1], points[3][2]);

	// Left

	// Back

	// Right

	// Bottom

	glEnd();
	glPopMatrix();
}

void drawBuildingCube(const GLfloat(&trans)[3]) {

}

void drawBridge(const GLfloat(&trans)[3]) {

}

void display()
{
	glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	drawBase({ -0.5f, -0.5f, 0 });
	drawBase({ 0.5f, -0.5f, 0 });
	
	drawPillar({ -0.6f, 0.5f, 0.15f });
	drawPillar({ -0.4f, 0.5f, 0.15f });
	drawPillar({ -0.4f, 0.5f, -0.15f });
	drawPillar({ -0.6f, 0.5f, -0.15f });
	drawPillar({ 0.6f, 0.5f, 0.15f });
	drawPillar({ 0.4f, 0.5f, 0.15f });
	drawPillar({ 0.4f, 0.5f, -0.15f });
	drawPillar({ 0.6f, 0.5f, -0.15f });

	drawConeRoof({ -0.6f, 0.7f, 0.15f });
	drawConeRoof({ -0.4f, 0.7f, 0.15f });
	drawConeRoof({ -0.4f, 0.7f, -0.15f });
	drawConeRoof({ -0.6f, 0.7f, -0.15f });
	drawConeRoof({ 0.6f, 0.7f, 0.15f });
	drawConeRoof({ 0.4f, 0.7f, 0.15f });
	drawConeRoof({ 0.4f, 0.7f, -0.15f });
	drawConeRoof({ 0.6f, 0.7f, -0.15f });
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
