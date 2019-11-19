#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <conio.h>
#include <math.h>
#include <string>
#define PI 3.1415927

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
		{ 0.12f, 0, 0.15f }, // A [0][]
		{ -0.12f, 0, 0.15f }, // B [1][]
		{ -0.12f, 0, -0.15f }, // C [2][]
		{ 0.12f, 0, -0.15f }, // D [3][]
		{ 0, 0.45f, 0 }, // E [4][]
	};

	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);
	glBegin(GL_QUADS);

	// Bottom (ABCD)
	glVertex3f(points[0][0], points[0][1], points[0][2]);
	glVertex3f(points[1][0], points[1][1], points[1][2]);
	glVertex3f(points[2][0], points[2][1], points[2][2]);
	glVertex3f(points[3][0], points[3][1], points[3][2]);

	// Left (BCE)
	glVertex3f(points[1][0], points[1][1], points[1][2]);
	glVertex3f(points[2][0], points[2][1], points[2][2]);
	glVertex3f(points[4][0], points[4][1], points[4][2]);

	// Back (CDE)
	glVertex3f(points[2][0], points[2][1], points[2][2]);
	glVertex3f(points[3][0], points[3][1], points[3][2]);
	glVertex3f(points[4][0], points[4][1], points[4][2]);

	// Right (DAE)
	glVertex3f(points[3][0], points[3][1], points[3][2]);
	glVertex3f(points[0][0], points[0][1], points[0][2]);
	glVertex3f(points[4][0], points[4][1], points[4][2]);

	// Front (ABE)
	glVertex3f(points[0][0], points[0][1], points[0][2]);
	glVertex3f(points[1][0], points[1][1], points[1][2]);
	glVertex3f(points[2][0], points[2][1], points[2][2]);

	glEnd();
	glPopMatrix();
}

void drawMainTowerCube(const GLfloat(&trans)[3]) {
	float points[8][3] = {
		{ 0.1f, 0.3f, 0.15f }, // A [0][]
		{ 0.1f, -0.3f, 0.15f }, // B [1][]
		{ -0.1f, -0.3f, 0.15f }, // C [2][]
		{ -0.1f, 0.3f, 0.15f }, // D [3][]
		{ 0.1f, 0.3f, -0.15f }, // E [4][]
		{ 0.1f, -0.3f, -0.15f }, // F [5][]
		{ -0.1f, -0.3f, -0.15f }, // G [6][]
		{ -0.1f, 0.3f, -0.15f }, // H [7][]
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

void drawMainTowerBridge(const GLfloat(&trans)[3]) {
	float points[8][3] = {
		{ 0.2f, 0.01f, 0.01f }, // A [0][]
		{ 0.2f, -0.01f, 0.01f }, // B [1][]
		{ -0.2f, -0.01f, 0.01f }, // C [2][]
		{ -0.2f, 0.01f, 0.01f }, // D [3][]
		{ 0.2f, 0.01f, -0.01f }, // E [4][]
		{ 0.2f, -0.01f, -0.01f }, // F [5][]
		{ -0.2f, -0.01f, -0.01f }, // G [6][]
		{ -0.2f, 0.01f, -0.01f }, // H [7][]
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

void drawMainTowerSupport(const GLfloat(&trans)[3], const GLfloat(&rotateY)[4], GLfloat rotateZDeg1, GLfloat rotateZDeg2) {
	GLUquadricObj* var = NULL;
	
	var = gluNewQuadric();
	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);
	glRotatef(rotateY[0], rotateY[1], rotateY[2], rotateY[3]);
	gluQuadricDrawStyle(var, GLU_LINE);
	// gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0.001f, 0.001f, 0.4f, 32, 32);
	glPopMatrix();
	var = gluNewQuadric();
	glPushMatrix();
	glTranslatef(trans[0], trans[1] - 0.01f, trans[2]);
	glRotatef(rotateY[0], rotateY[1], rotateY[2], rotateY[3]);
	gluQuadricDrawStyle(var, GLU_LINE);
	// gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0.001f, 0.001f, 0.4f, 32, 32);
	glPopMatrix();

	var = gluNewQuadric();
	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);
	glRotatef(rotateZDeg1, 0, 0, 1);
	glRotatef(rotateY[0], rotateY[1], rotateY[2], rotateY[3]);
	gluQuadricDrawStyle(var, GLU_LINE);
	// gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0.001f, 0.001f, 0.4f, 32, 32);
	glPopMatrix();

	var = gluNewQuadric();
	glPushMatrix();
	glTranslatef(trans[0], trans[1] - 0.1f, trans[2]);
	glRotatef(rotateZDeg2, 0, 0, 1);
	glRotatef(rotateY[0], rotateY[1], rotateY[2], rotateY[3]);
	gluQuadricDrawStyle(var, GLU_LINE);
	// gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0.001f, 0.001f, 0.4f, 32, 32);
	glPopMatrix();

	var = gluNewQuadric();
	glPushMatrix();
	glTranslatef(trans[0], trans[1] - 0.1f, trans[2]);
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(var, GLU_LINE);
	// gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0.001f, 0.001f, 0.1f, 32, 32);
	glPopMatrix();
	var = gluNewQuadric();
	glPushMatrix();
	glTranslatef(trans[0] - 0.01f, trans[1] - 0.1f, trans[2]);
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(var, GLU_LINE);
	// gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0.001f, 0.001f, 0.1f, 32, 32);
	glPopMatrix();
}

void drawRoad(const GLfloat(&trans)[3]) {
	float points[8][3] = {
		{ 0.35f, 0.201f, 0.1f }, // A [0][]
		{ 0.35f, -0.2f, 0.1f }, // B [1][]
		{ -0.35f, -0.2f, 0.1f }, // C [2][]
		{ -0.35f, 0.201f, 0.1f }, // D [3][]
		{ 0.35f, 0.201f, -0.1f }, // E [4][]
		{ 0.35f, -0.2f, -0.1f }, // F [5][]
		{ -0.35f, -0.2f, -0.1f }, // G [6][]
		{ -0.35f, 0.201f, -0.1f }, // H [7][]
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

void drawRoadBridge(const GLfloat(&trans)[3], const GLfloat(&rotate)[4]) {
	float points[8][3] = {
		{ 0.15f, 0.025f, 0.1f }, // A [0][]
		{ 0.15f, -0.025f, 0.1f }, // B [1][]
		{ -0.15f, -0.025f, 0.1f }, // C [2][]
		{ -0.15f, 0.025f, 0.1f }, // D [3][]
		{ 0.15f, 0.025f, -0.1f }, // E [4][]
		{ 0.15f, -0.025f, -0.1f }, // F [5][]
		{ -0.15f, -0.025f, -0.1f }, // G [6][]
		{ -0.15f, 0.025f, -0.1f }, // H [7][]
	};

	glPushMatrix();
	glRotatef(rotate[0], rotate[1], rotate[2], rotate[3]);
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
	glRotatef(rotate[0], rotate[1], rotate[2], rotate[3]);
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
	glRotatef(rotate[0], rotate[1], rotate[2], rotate[3]);
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

void drawRoadBridgeSupport(const GLfloat(&trans)[3], const GLfloat(&rotateX)[4], const GLfloat(&rotateY)[4]) {
	GLUquadricObj* var = NULL;
	var = gluNewQuadric();

	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);
	glRotatef(rotateY[0], rotateY[1], rotateY[2], rotateY[3]);
	glRotatef(rotateX[0], rotateX[1], rotateX[2], rotateX[3]);
	gluQuadricDrawStyle(var, GLU_LINE);
	// gluQuadricTexture(var, GL_TRUE);
	gluCylinder(var, 0.01f, 0.01f, 0.3f, 32, 32);
	glPopMatrix();
}

void draw_cylinder(GLfloat radius, GLfloat height, GLubyte R, GLubyte G, GLubyte B)
{
	GLfloat w0, w1, ang0, ang1, angle, x, y, xb, yb, zb;
	int i, j;

	int     slices = 8;
	GLfloat bend_radius = 1.0f;

	GLfloat bend_angle, bend_ang0, bend_ang1;

	bend_angle = bend_radius * height;
	bend_ang0 = -bend_angle / 2.0f;
	bend_ang1 = bend_angle / 2.0f;

	for (i = 0; i < slices; i++)
	{
		w0 = (float)i / (float)slices;
		w1 = (float)(i + 1) / (float)slices;

		ang0 = bend_ang0 + (bend_ang1 - bend_ang0) * w0;
		ang1 = bend_ang0 + (bend_ang1 - bend_ang0) * w1;

		glColor3f(1.0f - w0, 0.0, w1);
		glBegin(GL_QUAD_STRIP);

		for (j = 0; j <= 360; ++j)
		{
			angle = PI * (float)j * PI / 180.0f;
			x = radius * cos(angle) + bend_radius;
			y = radius * sin(angle);

			xb = sin(ang0) * x;
			yb = y;
			zb = cos(ang0) * x;
			glVertex3f(xb, yb, zb);

			xb = sin(ang1) * x;
			yb = y;
			zb = cos(ang1) * x;
			glVertex3f(xb, yb, zb);
		}
		glEnd();
	}
}

void display()
{
	glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	//drawBase({ -0.5f, -0.5f, 0 });
	//drawBase({ 0.5f, -0.5f, 0 });
	//
	//drawPillar({ -0.6f, 0.5f, 0.15f });
	//drawPillar({ -0.4f, 0.5f, 0.15f });
	//drawPillar({ -0.4f, 0.5f, -0.15f });
	//drawPillar({ -0.6f, 0.5f, -0.15f });
	//drawPillar({ 0.6f, 0.5f, 0.15f });
	//drawPillar({ 0.4f, 0.5f, 0.15f });
	//drawPillar({ 0.4f, 0.5f, -0.15f });
	//drawPillar({ 0.6f, 0.5f, -0.15f });
	//
	//drawMainTowerCube({ -0.5f, 0.2f, 0 });
	//drawMainTowerCube({ 0.5f, 0.2f, 0 });
	//
	//drawMainTowerBridge({ -0.2f, 0.3f, 0.1f });
	//drawMainTowerBridge({ 0.2f, 0.3f, 0.1f });
	//drawMainTowerBridge({ -0.2f, 0.3f, -0.1f });
	//drawMainTowerBridge({ 0.2f, 0.3f, -0.1f });
	//
	//drawMainTowerSupport({ 0, 0.4f, 0.1f }, { -90, 0, 1, 0 }, 12, -12);
	//drawMainTowerSupport({ 0, 0.4f, 0.1f }, { 90, 0, 1, 0 }, -12, 12);
	//drawMainTowerSupport({ 0, 0.4f, -0.1f }, { -90, 0, 1, 0 }, 12, -12);
	//drawMainTowerSupport({ 0, 0.4f, -0.1f }, { 90, 0, 1, 0 }, -12, 12);
	//
	//drawConeRoof({ -0.6f, 0.7f, 0.15f });
	//drawConeRoof({ -0.4f, 0.7f, 0.15f });
	//drawConeRoof({ -0.4f, 0.7f, -0.15f });
	//drawConeRoof({ -0.6f, 0.7f, -0.15f });
	//drawConeRoof({ 0.6f, 0.7f, 0.15f });
	//drawConeRoof({ 0.4f, 0.7f, 0.15f });
	//drawConeRoof({ 0.4f, 0.7f, -0.15f });
	//drawConeRoof({ 0.6f, 0.7f, -0.15f });
	//
	//drawPyramidRoof({ -0.5f, 0.5f, 0 });
	//drawPyramidRoof({ 0.5f, 0.5f, 0 });
	//
	//drawRoad({ 0.65f, -0.5f, 0 });
	//drawRoad({ -0.65f, -0.5f, 0 });
	//
	//drawRoadBridge({ -0.23f, -0.235f, 0 }, { 15, 0, 0, 1 });
	//drawRoadBridge({ 0.23f, -0.235f, 0 }, { -15, 0, 0, 1 });
	//
	//drawRoadBridgeSupport({ -0.05f, -0.27f, 0 }, { 23, 1, 0, 0 }, { -90, 0, 1, 0 });
	//drawRoadBridgeSupport({ 0.05f, -0.27f, 0 }, { 23, 1, 0, 0 }, { 90, 0, 1, 0 });

	draw_cylinder(0.15, 1.0, 255, 160, 100);
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
