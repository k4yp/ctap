#include "libs.h"

#define SW         480             //screen width
#define SH         360             //screen height
#define SW2        (SW/2)          //half of screen width
#define SH2        (SH/2)          //half of screen height
#define pixelScale 2               //OpenGL pixel scale
#define GLSW       (SW*pixelScale) //OpenGL window width
#define GLSH       (SH*pixelScale) //OpenGL window height

#define PI        3.14159265358979323846

struct Time T;

struct Keys K;

struct Math M;

struct Player P;

void drawPixel(int x, int y, float r, float g, float b)
{
	glBegin(GL_POINTS);
	glColor3f(r, g, b);
	glVertex2i(x * pixelScale, y * pixelScale);
	glEnd();
}

void movePlayer()
{
	// player look left, right, up, down
	if (K.h == 1)
	{
		P.a -= 1;
		if (P.a < 0)
		{
			P.a += 360;
		}
	}
	if (K.l == 1)
	{
		P.a += 1;
		if (P.a > 359)
		{
			P.a -= 360;
		}
	}
	if (K.j == 1)
	{
		P.l -= 1;
	}
	if (K.k == 1)
	{
		P.l += 1;
	}

	int dx = M.sin[P.a] * 2.0;
	int dy = M.cos[P.a] * 2.0;
	
	//player move forwards, backwards, left, right
	if (K.w == 1)
	{
		P.x += dx;
		P.y += dy;
	}
	if (K.s == 1) 
	{ 
		P.x -= dx;
		P.y -= dy; 
	}
	if (K.a == 1)
	{
		P.x -= dy;
		P.y += dx;
	}
	if (K.d == 1)
	{
		P.x += dy;
		P.y -= dx; 
	}

	//player move up, down
	if (K.z == 1)
	{
		P.z += 2;
	}
	if (K.x == 1)
	{
		P.z -= 2;
	}
}

void drawWall(int x1,int x2, int b1, int b2, int t1, int t2) 
{
	int x, y;

	//difference in distance
	int dyb = b2 - b1;
	int dyt = t2 - t1;
	int dx = x2 - x1;
	if (dx == 0)
	{
		dx = 1;
	}
	int xs = x1;

	//clip x
	if (x1 < 1)
	{
		x1 = 1;
	}
	if (x2 < 1)
	{
		x2 = 1;
	}
	if (x1 > SW - 1)
	{
		x1 = SW-1;
	}
	if (x2 > SW - 1)
	{
		x2 = SW - 1;
	}

	//clip x
	if (x1 < 1)
	{
		x1 = 1;
	}
	if (x2 < 1)
	{
		x2 = 1;
	}
	if (x1 > SW - 1)
	{
		x1 = SW - 1;
	}
	if (x2 > SW - 1)
	{
		x2 = SW - 1;
	}

	for (x = x1; x < x2; x++) 
	{
		int y1 = dyb * (x - xs + 0.5) / dx + b1;
		int y2 = dyt * (x - xs + 0.5) / dx + t1;

		//clip y
		if (y1 < 1)
		{
			y1 = 1;
		}
		if (y2 < 1)
		{
			y2 = 1;
		}
		if (y1 > SW - 1)
		{
			y1 = SW - 1;
		}
		if (y2 > SW - 1)
		{
			y2 = SW - 1;
		}

		for (y = y1; y < y2; y++) 
		{
			drawPixel(x, y, 0.5, 0.5, 0.0);
		}
	}
}

void clipBehindPlayer(int *x1, int *y1, int *z1, int x2, int y2, int z2)
{
	float da = *y1;
	float db = y2;
	float d = da - db;
	if (d == 0)
	{
		d = 1; 
	}
	float s = da / (da - db);
	*x1 = *x1 + s * (x2 - (*x1));
	*y1 = *y1 + s * (y2 - (*y1)); 
	if (*y1 == 0)
	{
		*y1 = 1; 
	} 
	*z1 =  *z1 + s*(z2 - (*z1));
}

void draw3D()
{
	int wx[4], wy[4], wz[4];

	float CS = M.cos[P.a];
	float SN = M.sin[P.a];

	int x1 = 40 - P.x;
	int y1 = 10 - P.y;
	int x2 = 40 - P.x;
	int y2 = 290 - P.y;

	//world x postion
	wx[0] = x1 * CS - y1 * SN;
	wx[1] = x2 * CS - y2 * SN;

	wx[2] = wx[0];
	wx[3] = wx[1];

	//world y position
	wy[0] = y1 * CS + x1 * SN;
	wy[1] = y2 * CS + x2 * SN;

	wy[2] = wy[0];
	wy[3] = wy[1];

	//world z position
	wz[0] = 0 - P.z + ((P.l * wy[0]) / 32.0);
	wz[1] = 0 - P.z + ((P.l * wy[1]) / 32.0);

	wz[2] = wz[0] + 40;
	wz[3] = wz[1] + 40;


	if (wy[0] < 1)
	{
		clipBehindPlayer(&wx[0], &wy[0], &wz[0], wx[1], wy[1], wz[1]);
		clipBehindPlayer(&wx[2], &wy[2], &wz[2], wx[3], wy[3], wz[3]);
	}

	if (wy[1] < 1)
	{
		clipBehindPlayer(&wx[1], &wy[1], &wz[1], wx[0], wy[0], wz[0]);
		clipBehindPlayer(&wx[3], &wy[3], &wz[3], wx[2], wy[2], wz[2]);
	}

	//screen x, y position
	wx[0] = wx[0] * 200 / wy[0] + SW2;
	wy[0] = wz[0] * 200 / wy[0] + SH2;

	wx[1] = wx[1] * 200 / wy[1] + SW2;
	wy[1] = wz[1] * 200 / wy[1] + SH2;

	wx[2] = wx[2] * 200 / wy[2] + SW2;
	wy[2] = wz[2] * 200 / wy[2] + SH2;

	wx[3] = wx[3] * 100 / wy[3] + SW2;
	wy[3] = wz[3] * 200 / wy[3] + SH2;

	drawWall(wx[0], wx[1], wy[0], wy[1], wy[2], wy[3]);
}

void display()
{
	if (T.fr1 - T.fr2 >= 8)
	{
		glClearColor(0.0, 0.0, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		movePlayer();
		draw3D();

		T.fr2 = T.fr1;
		glutSwapBuffers();
		glutReshapeWindow(GLSW, GLSH);
	}

	T.fr1 = glutGet(GLUT_ELAPSED_TIME);
	glutPostRedisplay();
}

void KeysDown(unsigned char key, int x, int y)
{
	if (key == 'h' == 1)
	{
		K.h = 1;
	}
	if (key == 'l' == 1)
	{
		K.l = 1;
	}
	if (key == 'j' == 1)
	{
		K.j = 1;
	}
	if (key == 'k' == 1)
	{
		K.k = 1;
	}
	if (key == 'w' == 1)
	{
		K.w = 1;
	}
	if (key == 's' == 1)
	{
		K.s = 1;
	}
	if (key == 'a' == 1)
	{
		K.a = 1;
	}
	if (key == 'd' == 1)
	{
		K.d = 1;
	}
	if (key == 'z' == 1)
	{
		K.z = 1;
	}
	if (key == 'x' == 1)
	{
		K.x = 1;
	}
}

void KeysUp(unsigned char key, int x, int y)
{
	if (key == 'h' == 1)
	{
		K.h = 0;
	}
	if (key == 'l' == 1)
	{
		K.l = 0;
	}
	if (key == 'j' == 1)
	{
		K.j = 0;
	}
	if (key == 'k' == 1)
	{
		K.k = 0;
	}
	if (key == 'w' == 1)
	{
		K.w = 0;
	}
	if (key == 's' == 1)
	{
		K.s = 0;
	}
	if (key == 'a' == 1)
	{
		K.a = 0;
	}
	if (key == 'd' == 1)
	{
		K.d = 0;
	}
	if (key == 'z' == 1)
	{
		K.z = 0;
	}
	if (key == 'x' == 1)
	{
		K.x = 0;
	}
}

void init()
{
	for (int i = 0; i < 360; i++) {
		M.cos[i] = cos(i / 180.0 * PI);
		M.sin[i] = sin(i / 180.0 * PI);
	}

	P.x = 80;
	P.y = -110;
	P.z = 20;
	P.a = 0;
	P.l = 0;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(GLSW, GLSH);
	glutCreateWindow("ctap");
	glPointSize(pixelScale);
	gluOrtho2D(0, GLSW, 0, GLSH);
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(KeysDown);
	glutKeyboardUpFunc(KeysUp);
	glutMainLoop();
	return 0;
}