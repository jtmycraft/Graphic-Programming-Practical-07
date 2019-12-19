#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <conio.h>
#include <string>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "P6"
#define PI 3.141592654f

// Mode
int currentMode = 0;

// Screen
const float screenWidth = 800, screenHeight = 600;

// Camera
float orthoLeft = -1.0f, orthoRight = 1.0f, orthoBottom = -1.0f, orthoTop = 1.0f, orthoZNear = 0, orthoZFar = 10.0f;
float prespFovy = 60;
float prespLeft = -1.0f, prespRight = 1.0f, prespBottom = -1.0f, prespTop = 1.0f, prespZNear = 0, prespZFar = 10.0f;
float lastCursorPosX, lastCursorPosY;
float cameraPosX = 0, cameraPosY = 0, cameraPosZ = 0, cameraRotX = 0, cameraRotY = 0;
POINT cursorPos;

// Lighting
float lightPos[] = { 0, 1, 0 };
float lightMovementSpd = 0.1f;
float lightDif[] = { 1, 0, 0 };

// Model
float modelColor[] = { 0.2f, 0.2f, 0.2f };
float modelSize = 0.5f; // Default size for radius of circle (Cone, Cylinder, Circle) or half size of other 3D object (Cube, Pyramid).
float modelSegment = 16; // For circle object use (Number of Stack, Number of Slice)

						 // Color
const float colorWhite[] = { 255, 255, 255 };
const float colorRed[] = { 255, 0, 0 };
const float colorGreen[] = { 0, 255, 0 };
const float colorBlue[] = { 0, 0, 255 };

// Texture
GLuint texture = 0;
BITMAP BMP;
HBITMAP hBMP = NULL;

// temp
float rot = 0;

void setTexture(LPCSTR bmp_dir);

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CHAR:
		switch (currentMode) {
		case 0: // Ortho Camera
			switch (wParam) {
			case 0x41: // A L-
				orthoLeft--;
				break;
			case 0x61: // a L+
				orthoLeft++;
				break;

			case 0x44: // D R+
				orthoRight++;
				break;
			case 0x64: // d R-
				orthoRight--;
				break;

			case 0x57: // W T+
				orthoTop++;
				break;
			case 0x77: // w T-
				orthoTop--;
				break;

			case 0x53: // S B-
				orthoBottom--;
				break;
			case 0x73: // s B+
				orthoBottom++;
				break;

			case 0x51: // Q N+
				orthoZNear++;
				break;
			case 0x71: // q N-
				if (orthoZNear > 0) {
					orthoZNear--;
				}
				break;

			case 0x45: // E F+
				orthoZFar++;
				break;
			case 0x65: // e F-
				if (orthoZFar > 0) {
					orthoZFar--;
				}
				break;
			}
			break;
		case 1: // Presp Camera
			switch (wParam) {
			case 0x5A: // FOVY-
				prespFovy--;
				break;

			case 0x43: // FOVY+
				prespFovy++;
				break;
			}
			break;
		}

		break;

	case WM_KEYDOWN:
		float xrotrad, yrotrad;

		// Switch current mode
		switch (wParam) {
		case 0x31: // Camera Mode
			currentMode = 0;
			break;
		case 0x32:
			currentMode = 1;
			break;

			// Light Movement
		case VK_NUMPAD8:
			lightPos[1] += lightMovementSpd;
			break;
		case VK_NUMPAD2:
			lightPos[1] -= lightMovementSpd;
			break;
		case VK_NUMPAD4:
			lightPos[0] -= lightMovementSpd;
			break;
		case VK_NUMPAD6:
			lightPos[0] += lightMovementSpd;
			break;
		case VK_NUMPAD7:
			lightPos[2] += lightMovementSpd;
			break;
		case VK_NUMPAD9:
			lightPos[2] -= lightMovementSpd;
			break;

			// Eye movement
		case 0x57: // Ww
			yrotrad = (cameraRotY / 180 * PI);
			xrotrad = (cameraRotX / 180 * PI);
			cameraPosX += float(sin(yrotrad)) * 0.05;
			cameraPosZ -= float(cos(yrotrad)) * 0.05;
			cameraPosY -= float(sin(xrotrad)) * 0.05;
			break;
		case 0x53: // Ss
			yrotrad = (cameraRotY / 180 * PI);
			xrotrad = (cameraRotX / 180 * PI);
			cameraPosX -= float(sin(yrotrad)) * 0.05;
			cameraPosZ += float(cos(yrotrad)) * 0.05;
			cameraPosY += float(sin(xrotrad)) * 0.05;
			break;
		case 0x41: // Aa
			yrotrad = (cameraRotY / 180 * PI);
			cameraPosX -= float(cos(yrotrad)) * 0.05;
			cameraPosZ -= float(sin(yrotrad)) * 0.05;
			break;
		case 0x44: // Dd
			yrotrad = (cameraRotY / 180 * PI);
			cameraPosX += float(cos(yrotrad)) * 0.05;
			cameraPosZ += float(sin(yrotrad)) * 0.05;
			break;
		case VK_SPACE:
			cameraPosY += lightMovementSpd;
			break;
		case VK_CONTROL:
			cameraPosY -= lightMovementSpd;
			break;
		case VK_UP:
			rot++;
			break;
		case VK_DOWN:
			rot--;
		case 0x37:
			setTexture("res/BrickFacade0003_1_seamless_S.bmp");
			break;
		case 0x38:
			setTexture("res/Cargo0095_S.bmp");
			break;
		case 0x39:
			setTexture("res/Metal_AluminumBrushed2_512_albedo.bmp");
			break;
		}
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
/*
Basic Shape
*/

void drawCircle(float cx, float cy, float r, int segmentsNum) {
	glBegin(GL_LINE_LOOP); //(GL_POLYGON);
	for (int ii = 0; ii < segmentsNum; ii++) {
		float theta = 2.0f * 3.1415926f * float(ii) / float(segmentsNum);//get the current angle 
		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 
		glVertex2f(x + cx, y + cy);//output vertex 
	}
	glEnd();
}

void drawSphere() {
	GLUquadricObj* var = gluNewQuadric();
	gluQuadricDrawStyle(var, GLU_LINE);
	gluSphere(var, modelSize, modelSegment, modelSegment);
	gluDeleteQuadric(var);
}

void drawCone() {
	glPushMatrix();
	{
		GLUquadricObj* var = gluNewQuadric();
		gluQuadricDrawStyle(var, GLU_LINE);
		gluCylinder(var, modelSize, 0, modelSize, modelSegment, modelSegment);
		gluDeleteQuadric(var);

		// Draw Top & Bottom cover.
		drawCircle(0, 0, modelSize, modelSegment);
	}
	glPopMatrix();
}

void drawCylinder() {
	glPushMatrix();
	{
		GLUquadricObj* var = gluNewQuadric();
		gluQuadricDrawStyle(var, GLU_LINE);
		gluCylinder(var, modelSize, modelSize, modelSize, modelSegment, modelSegment);
		gluDeleteQuadric(var);

		// Draw Top & Bottom cover.
		drawCircle(0, 0, modelSize, modelSegment);
		glTranslatef(0, 0, 0.5f);
		drawCircle(0, 0, modelSize, modelSegment);
	}
	glPopMatrix();
}

void drawPyramid() {
	float points[5][3] = {
		{ modelSize, 0, modelSize }, // A [0][]
	{ -modelSize, 0, modelSize }, // B [1][]
	{ -modelSize, 0, -modelSize }, // C [2][]
	{ modelSize, 0, -modelSize }, // D [3][]
	{ 0, modelSize, 0 }, // E [4][]
	};

	glBegin(GL_QUADS);
	{
		// Bottom (ABCD)
		glVertex3f(points[0][0], points[0][1], points[0][2]);
		glVertex3f(points[1][0], points[1][1], points[1][2]);
		glVertex3f(points[2][0], points[2][1], points[2][2]);
		glVertex3f(points[3][0], points[3][1], points[3][2]);
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	{
		// Left (BCE)
		glVertex3f(points[1][0], points[1][1], points[1][2]);
		glVertex3f(points[2][0], points[2][1], points[2][2]);
		glVertex3f(points[4][0], points[4][1], points[4][2]);

	}
	glEnd();

	glBegin(GL_TRIANGLES);
	{
		// Back (CDE)
		glVertex3f(points[2][0], points[2][1], points[2][2]);
		glVertex3f(points[3][0], points[3][1], points[3][2]);
		glVertex3f(points[4][0], points[4][1], points[4][2]);

	}
	glEnd();

	glBegin(GL_TRIANGLES);
	{
		// Right (DAE)
		glVertex3f(points[3][0], points[3][1], points[3][2]);
		glVertex3f(points[0][0], points[0][1], points[0][2]);
		glVertex3f(points[4][0], points[4][1], points[4][2]);
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	{
		// Front (ABE)
		glVertex3f(points[0][0], points[0][1], points[0][2]);
		glVertex3f(points[1][0], points[1][1], points[1][2]);
		glVertex3f(points[4][0], points[4][1], points[4][2]);
	}
	glEnd();
}

void drawCube() {
	float points[8][3] = {
		{ modelSize, modelSize, modelSize }, // A [0][]
	{ modelSize, -modelSize, modelSize }, // B [1][]
	{ -modelSize, -modelSize, modelSize }, // C [2][]
	{ -modelSize, modelSize, modelSize }, // D [3][]
	{ modelSize, modelSize, -modelSize }, // E [4][]
	{ modelSize, -modelSize, -modelSize }, // F [5][]
	{ -modelSize, -modelSize, -modelSize }, // G [6][]
	{ -modelSize, modelSize, -modelSize }, // H [7][]
	};



	glBegin(GL_QUADS);
	{
		// Front (ABCD)
		glTexCoord2f(1.0f, 1.0f); glVertex3f(points[0][0], points[0][1], points[0][2]);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(points[1][0], points[1][1], points[1][2]);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(points[2][0], points[2][1], points[2][2]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(points[3][0], points[3][1], points[3][2]);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		// Back (EFGH)
		glTexCoord2f(1.0f, 1.0f); glVertex3f(points[4][0], points[4][1], points[4][2]);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(points[5][0], points[5][1], points[5][2]);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(points[6][0], points[6][1], points[6][2]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(points[7][0], points[7][1], points[7][2]);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		// Left (CDHg)
		glTexCoord2f(1.0f, 1.0f); glVertex3f(points[2][0], points[2][1], points[2][2]);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(points[3][0], points[3][1], points[3][2]);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(points[7][0], points[7][1], points[7][2]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(points[6][0], points[6][1], points[6][2]);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		// Right (ABFE)
		glTexCoord2f(1.0f, 1.0f); glVertex3f(points[0][0], points[0][1], points[0][2]);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(points[1][0], points[1][1], points[1][2]);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(points[5][0], points[5][1], points[5][2]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(points[4][0], points[4][1], points[4][2]);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		// Top (ADHE)
		glTexCoord2f(1.0f, 1.0f); glVertex3f(points[0][0], points[0][1], points[0][2]);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(points[3][0], points[3][1], points[3][2]);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(points[7][0], points[7][1], points[7][2]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(points[4][0], points[4][1], points[4][2]);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		// Bottom (BCGF)
		glTexCoord2f(1.0f, 1.0f); glVertex3f(points[1][0], points[1][1], points[1][2]);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(points[2][0], points[2][1], points[2][2]);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(points[6][0], points[6][1], points[6][2]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(points[5][0], points[5][1], points[5][2]);
	}
	glEnd();
}

void drawEquilateralPrism() {
	float points[6][3] = {
		{ 0, modelSize, modelSize }, // A [0][]
	{ modelSize, -modelSize, modelSize }, // B [1][]
	{ -modelSize, -modelSize, modelSize }, // C [2][]
	{ 0, modelSize, -modelSize }, // D [3][]
	{ modelSize, -modelSize, -modelSize }, // E [4][]
	{ -modelSize, -modelSize, -modelSize }, // F [5][]
	};

	glBegin(GL_LINE_LOOP); // ABC
	{
		glVertex3fv(points[0]);
		glVertex3fv(points[1]);
		glVertex3fv(points[2]);
	}
	glEnd();

	glBegin(GL_LINE_LOOP); // DEF
	{
		glVertex3fv(points[3]);
		glVertex3fv(points[4]);
		glVertex3fv(points[5]);
	}
	glEnd();

	glBegin(GL_LINE_LOOP); // ABED
	{
		glVertex3fv(points[0]);
		glVertex3fv(points[1]);
		glVertex3fv(points[4]);
		glVertex3fv(points[3]);
	}
	glEnd();

	glBegin(GL_LINE_LOOP); // ACFD
	{
		glVertex3fv(points[0]);
		glVertex3fv(points[2]);
		glVertex3fv(points[5]);
		glVertex3fv(points[3]);
	}
	glEnd();

	glBegin(GL_LINE_LOOP); // BCFE
	{
		glVertex3fv(points[1]);
		glVertex3fv(points[2]);
		glVertex3fv(points[5]);
		glVertex3fv(points[4]);
	}
	glEnd();
}

void drawRightPrism() {
	float points[6][3] = {
		{ -modelSize, modelSize, modelSize }, // A [0][]
	{ modelSize, -modelSize, modelSize }, // B [1][]
	{ -modelSize, -modelSize, modelSize }, // C [2][]
	{ -modelSize, modelSize, -modelSize }, // D [3][]
	{ modelSize, -modelSize, -modelSize }, // E [4][]
	{ -modelSize, -modelSize, -modelSize }, // F [5][]
	};

	glBegin(GL_LINE_LOOP); // ABC
	{
		glVertex3fv(points[0]);
		glVertex3fv(points[1]);
		glVertex3fv(points[2]);
	}
	glEnd();

	glBegin(GL_LINE_LOOP); // DEF
	{
		glVertex3fv(points[3]);
		glVertex3fv(points[4]);
		glVertex3fv(points[5]);
	}
	glEnd();

	glBegin(GL_LINE_LOOP); // ABED
	{
		glVertex3fv(points[0]);
		glVertex3fv(points[1]);
		glVertex3fv(points[4]);
		glVertex3fv(points[3]);
	}
	glEnd();

	glBegin(GL_LINE_LOOP); // ACFD
	{
		glVertex3fv(points[0]);
		glVertex3fv(points[2]);
		glVertex3fv(points[5]);
		glVertex3fv(points[3]);
	}
	glEnd();

	glBegin(GL_LINE_LOOP); // BCFE
	{
		glVertex3fv(points[1]);
		glVertex3fv(points[2]);
		glVertex3fv(points[5]);
		glVertex3fv(points[4]);
	}
	glEnd();
}

//--------------------------------------------------------------------
/*
Settings
*/

void setLighting() {
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, colorBlue);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorWhite);
}

void setCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (currentMode) {
	case 0:
		//glOrtho(orthoLeft, orthoRight, orthoBottom, orthoTop, -orthoZNear, -orthoZFar);
		gluPerspective(prespFovy, screenWidth / screenHeight, 0.001, 1000);
		gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0);
		break;
	case 1:
		gluPerspective(prespFovy, screenWidth / screenHeight, 0.001, 1000);
		glFrustum(prespLeft, prespRight, prespBottom, prespTop, prespZNear, prespZFar);

		if (GetCursorPos(&cursorPos)) {
			int diffCursorPosX = cursorPos.x - lastCursorPosX; //check the difference between the current x and the last x position
			int diffCursorPosY = cursorPos.y - lastCursorPosY; //check the difference between the current y and the last y position
			lastCursorPosX = cursorPos.x; //set lastCursorPosX to the current x position
			lastCursorPosY = cursorPos.y; //set lastCursorPosY to the current y position
			cameraRotX += (float)diffCursorPosY; //set the cameraRotX to cameraRotX with the addition of the difference in the y position
			cameraRotY += (float)diffCursorPosX;    //set the cameraRotX to cameraRotY with the addition of the difference in the x position
			if (cameraRotX > 360) cameraRotX -= 360;
			if (cameraRotY > 360) cameraRotY -= 360;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(cameraRotX, 1.0, 0.0, 0.0);  //rotate our camera on the x-axis (left and right)
		glRotatef(cameraRotY, 0.0, 1.0, 0.0);  //rotate our camera on the y-axis (up and down)
		glTranslated(-cameraPosX, -cameraPosY, -cameraPosZ); //translate the screen to the position of our camera

		break;
	}
}

void setAxis(float scaleSize) {

	float points[4][3] = { // Arrow (ACBCD)
		{ modelSize - 0.05f, 0.05f, 0 }, // A [0][]
	{ modelSize - 0.05f, -0.05f, 0 }, // B [1][]
	{ modelSize, 0, 0 }, // C [2][]
	{ -modelSize, 0, 0 } // D [3][]
	};

	// x-axis
	glPushMatrix();
	{
		glScalef(scaleSize, 1, 1);
		glColor3fv(colorRed);
		glBegin(GL_LINE_STRIP);
		{
			glVertex3fv(points[0]);
			glVertex3fv(points[2]);
			glVertex3fv(points[1]);
			glVertex3fv(points[2]);
			glVertex3fv(points[3]);
		}
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	{
		// y-axis
		glScalef(1, scaleSize, 1);
		glRotatef(90, 0, 0, 1);
		glColor3fv(colorGreen);
		glBegin(GL_LINE_STRIP);
		{
			glVertex3fv(points[0]);
			glVertex3fv(points[2]);
			glVertex3fv(points[1]);
			glVertex3fv(points[2]);
			glVertex3fv(points[3]);
		}
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	{
		// z-axis
		glScalef(1, 1, scaleSize);
		glRotatef(-90, 0, 1, 0);
		glColor3fv(colorBlue);
		glBegin(GL_LINE_STRIP);
		{
			glVertex3fv(points[0]);
			glVertex3fv(points[2]);
			glVertex3fv(points[1]);
			glVertex3fv(points[2]);
			glVertex3fv(points[3]);
		}
		glEnd();
	}
	glPopMatrix();

	// Reset to white color (Due to unknown reason affect other object color).
	glColor3fv(colorWhite);
}

void setTexture(LPCSTR bmp_dir) {
	// Init
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		bmp_dir, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE); // First Param can change to bump.
	GetObject(hBMP, sizeof(BMP), &BMP);
}

//--------------------------------------------------------------------
/*
Construction
*/


void display() {
	glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	setAxis(6);
	setCamera();

	//// Draw Sun
	//glPushMatrix();
	//{
	//	glColor3f(1, 1, 1);
	//	glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
	//	glScalef(0.1f, 0.1f, 0.1f);
	//	drawSphere();
	//}
	//glPopMatrix();
	//
	//setLighting();

	// Draw


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
		BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	glPushMatrix();
	{
		if (rot < 360) rot += 0.05f;
		else rot = 0.05f;
		glRotatef(rot, 1, 1, 1);
		drawCube();
	}
	glPopMatrix();
	// --------------------
	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &texture);
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

	//setTexture();

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

	DeleteObject(hBMP);

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}

//--------------------------------------------------------------------
