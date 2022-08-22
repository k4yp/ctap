#include "libs.h"

#define SW         320                  //screen width
#define SH         180                  //screen height
#define SW2        (SW/2)               //half of screen width
#define SH2        (SH/2)               //half of screen height
#define pixelScale 4                    //OpenGL pixel scale
#define GLSW       (SW*pixelScale)      //OpenGL window width
#define GLSH       (SH*pixelScale)      //OpenGL window height

struct
{
	//frame 1, frame 2 to create constant frame rate
	int fr1, fr2;
}T;

struct
{
	//move up, down, left, right
	int w, s, a, d;

	//strafe left, right 
	int sl, sr;

	//move up, down, look up, down
	int m;
	int up, down, left, right;
}K;

//draw a pixel at x, y with rgb
void pixel(int x, int y, int color)
{
	int rgb[3];
	//red
	if (color == 0)
	{
		rgb[0] = 255;
		rgb[1] = 0;
		rgb[2] = 0;
	}
	//green
	if (color == 1)
	{
		rgb[0] = 0;
		rgb[1] = 255;
		rgb[2] = 0;
	}
	//blue
	if (color == 2)
	{
		rgb[0] = 0;
		rgb[1] = 0;
		rgb[2] = 255;
	}
	//white
	if (color == 3)
	{
		rgb[0] = 255;
		rgb[1] = 255;
		rgb[2] = 255;
	}
	//black
	if (color == 4)
	{
		rgb[0] = 0;
		rgb[1] = 0;
		rgb[2] = 0;
	}
	glBegin(GL_POINTS);
	glColor3f(rgb[0], rgb[1], rgb[2]);
	glVertex2i(x * pixelScale + 2, y * pixelScale + 2);
	glEnd();
}

void movePlayer()
{
	//move up, down, left, right
	if (K.a == 1 && K.m == 0) { printf("left\n"); }
	if (K.d == 1 && K.m == 0) { printf("right\n"); }
	if (K.w == 1 && K.m == 0) { printf("up\n"); }
	if (K.s == 1 && K.m == 0) { printf("down\n"); }
	//strafe left, right
	if (K.sr == 1) { printf("strafe left\n"); }
	if (K.sl == 1) { printf("strafe right\n"); }
	//move up, down, look up, look down
	if (K.a == 1 && K.m == 1) { printf("look up\n"); }
	if (K.d == 1 && K.m == 1) { printf("look down\n"); }
	if (K.w == 1 && K.m == 1) { printf("move up\n"); }
	if (K.s == 1 && K.m == 1) { printf("move down\n"); }
}

int tick;
void draw3D()
{
	int x = 0;
	int y = 0;
	int color = 0;
	for (y = 0; y < SH2; y++)
	{
		for (x = 0; x < SW2; x++)
		{
			pixel(x, y, color);
			color += 1;
			if (color == 5)
			{
				color = 0;
			}
		}
	}
	//frame rate
	tick += 1;
	if (tick > 20)
	{
		tick = 0;
	}
	pixel(SW2, SH2 + tick, 0);
}

void display()
{
	int x, y;
	if (T.fr1 - T.fr2 >= 1)
	{
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		movePlayer();
		draw3D();

		T.fr2 = T.fr1;
		glutSwapBuffers();
		glutReshapeWindow(GLSW, GLSH);             //prevent window scaling
	}

	T.fr1 = glutGet(GLUT_ELAPSED_TIME);          //1000 Milliseconds per second
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
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(GLSW / 2, GLSH / 2);
	glutInitWindowSize(GLSW, GLSH);
	glutCreateWindow("");
	glPointSize(pixelScale);
	gluOrtho2D(0, GLSW, 0, GLSH);
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(KeysDown);
	glutKeyboardUpFunc(KeysUp);
	glutMainLoop();
	return 0;
}