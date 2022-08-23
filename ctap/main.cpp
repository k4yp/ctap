#include "libs.h"

#define scale	   1
#define SW         640/scale       //screen width
#define SH         360/scale       //screen height
#define SW2        (SW/2)          //half of screen width
#define SH2        (SH/2)          //half of screen height
#define pixelScale 2*scale         //OpenGL pixel scale
#define GLSW       (SW*pixelScale) //OpenGL window width
#define GLSH       (SH*pixelScale) //OpenGL window height

#define PI        3.14159265358979323846

//time
struct
{
	//frame 1, frame 2 to create constant frame rate
	int fr1, fr2;
}T;

//keys
struct
{
	//move up, down, left, right
	int w, s, a, d;

	//strafe left, right 
	int sl, sr;

	//move up, down, look up, down
	int m;
}K;

//math
struct
{
	float cos[360];
	float sin[360];
}M;

//player
struct
{
	int x, y, z;
	int a;
	int l;
}P;

void drawPixel(int x, int y, int r, int g, int b)
{
	glBegin(GL_POINTS);
	glColor3f(r, g, b);
	glVertex2i(x * pixelScale + 2, y * pixelScale + 2);
	glEnd();
}

void movePlayer()
{
	//move up, down
	if (K.a == 1 && K.m == 0){
		P.a -= 4;
		if (P.a < 0) 
		{
			P.a += 360; 
		} 
	}
	if (K.d == 1 && K.m == 0) 
	{
		P.a += 4;
		if (P.a > 359) 
		{ 
			P.a -= 360; 
		} 
	}

	int dx = M.sin[P.a] * 10.0;
	int dy = M.cos[P.a] * 10.0;

	//move left, right
	if (K.w == 1 && K.m == 0) 
	{
		P.x += dx;
		P.y += dy; 
	}
	if (K.s == 1 && K.m == 0) 
	{ 
		P.x -= dx;
		P.y -= dy; 
	}

	//strafe left, right
	if (K.sr == 1)
	{
		P.x += dy;
		P.y -= dx; 
	}
	if (K.sl == 1) 
	{
		P.x -= dy;
		P.y += dx; 
	}

	//move up, down, look up, look down
	if (K.a == 1 && K.m == 1)
	{ 
		P.l -= 1;
	}
	if (K.d == 1 && K.m == 1)
	{
		P.l += 1; 
	}
	if (K.w == 1 && K.m == 1)
	{
		P.z -= 4; 
	}
	if (K.s == 1 && K.m == 1) 
	{ 
		P.z += 4;
	}
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

	//world y position
	wy[0] = y1 * CS + x1 * SN;
	wy[1] = y2 * CS + x2 * SN;

	//world z position
	wz[0] = 0 - P.z + ((P.l * wy[0]) / 32.0);
	wz[1] = 0 - P.z + ((P.l * wy[1]) / 32.0);

	//screen x, y position
	wx[0] = wx[0] * 200 / wy[0] + SW2;
	wy[0] = wz[0] * 200 / wy[0] + SH2;

	wx[1] = wx[1] * 200 / wy[1] + SW2;
	wy[1] = wz[1] * 200 / wy[1] + SH2;


	//draw points
	if (wx[0]>0 && wx[0]<SW && wy[0]>0 && wy[0]<SH)
	{
		drawPixel(wx[0], wy[0], 0, 255, 0);
	}

	if (wx[1]>0 && wx[1]<SW && wy[1]>0 && wy[1]<SH)
	{
		drawPixel(wx[1], wy[1], 255, 0, 0);
	}

}

void display()
{
	if (T.fr1 - T.fr2 >= 50)
	{
		glClearColor(0, 0, 0, 1.0f);
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
	if (key == 'm' == 1)
	{
		K.m = 1;
	}
	if (key == ',' == 1)
	{
		K.sr = 1;
	}
	if (key == '.' == 1)
	{
		K.sl = 1;
	}
}

void KeysUp(unsigned char key, int x, int y)
{
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
	if (key == 'm' == 1)
	{
		K.m = 0;
	}
	if (key == ',' == 1)
	{
		K.sr = 0;
	}
	if (key == '.' == 1)
	{
		K.sl = 0;
	}
}

void init()
{
	for (int i = 0; i < 360; i++) {
		M.cos[i] = cos(i / 180.0 * PI);
		M.sin[i] = sin(i / 180.0 * PI);
	}

	P.x = 70;
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