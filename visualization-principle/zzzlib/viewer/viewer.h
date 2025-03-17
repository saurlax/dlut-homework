#ifndef _VIEWER_H_
#define _VIEWER_H_

//#define GLUT_DISABLE_ATEXIT_HACK
#include <math.h>
#include<vector>
#include"../freeglut/glut.h"
#include"../freeglut/freeglut.h"
#include"Arcball.h"
#include"Point.h"
#include"quat.h"

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"


using namespace ViewerLib;

using namespace std;

/* window width and height */
int win_width, win_height;//record the current size of the window
int gButton;//record the current mouse button 
int startx, starty;//record the postion when the button was pushed down

/* rotation quaternion and translation vector for the object */
CQrot       ObjRot(0, 0, 1, 0);
CPoint      ObjTrans(0, 0, 0);

/* arcball object */
CArcball arcball;

/*variable for control*/
int show_mode = 0;

/*! setup the object, transform from the world to the object coordinate system */
void setupObject(void)
{
	double rot[16];

	glTranslated(ObjTrans[0], ObjTrans[1], ObjTrans[2]);
	ObjRot.convert(rot);

	glMultMatrixd((GLdouble*)rot);
}

/*! the eye is always fixed at world z = +5 */
void setupEye(void) {
	glLoadIdentity();//reset the trans matrix to indentity matrix
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);//we are facing the z direction and up direction is y axis
}

/*! draw mesh */
void draw_mesh()
{
	switch (show_mode)
	{
	case 0:
		glPointSize(10);
		glBegin(GL_POINTS);

		glVertex3f(1, 0, 0);

		glEnd();
		break;
	case 1:
		//draw line
		glLineWidth(5.0);
		glBegin(GL_LINES);


		glVertex3f(1, 0, 0);
		glColor3f(1, 0, 0);
		glVertex3f(0, 1, 0);
		glColor3f(0, 1, 0);

		glEnd();
		glLineWidth(5.0);
		break;
	case 2:

		//draw strip
		glLineWidth(5.0);
		glBegin(GL_LINE_STRIP);

		glColor3f(1, 0, 0);
		glVertex3f(1, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 1, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 1);

		glColor3f(1, 0, 0);
		glVertex3f(1, 0, 0);

		glEnd();
		glLineWidth(5.0);
		break;
	case 3:
		//draw Polygon
		glBegin(GL_POLYGON);

		glColor3f(1, 0, 0);
		glVertex3f(1, 0, 0);
		glNormal3d(1, 1, 1);

		glColor3f(0, 1, 0);
		glVertex3f(0, 1, 0);
		glNormal3d(1, 1, 1);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 1);
		glNormal3d(1, 1, 1);

		glEnd();
		break;

	case 4:

		//draw Polygon
		glBegin(GL_POLYGON);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(1, 0, 0);

		glColor3f(0, 0, 1);
		glVertex3f(1, 1, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 1, 0);

		glEnd();

		break;

	case 5:

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		glEnable(GL_TEXTURE_2D);

		//draw Polygon
		glBegin(GL_POLYGON);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2d(0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(1, 0, 0);
		glTexCoord2d(1, 0);

		glColor3f(0, 0, 1);
		glVertex3f(1, 1, 0);
		glTexCoord2d(1, 1);

		glColor3f(0, 0, 1);
		glVertex3f(0, 1, 0);
		glTexCoord2d(0, 1);

		glEnd();

		glDisable(GL_TEXTURE_2D);

		break;
	}
}

/*! display call back function*/
void display()
{
	/* clear frame buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* transform from the eye coordinate system to the world system */
	setupEye();
	glPushMatrix();
	/* transform from the world to the ojbect coordinate system */
	setupObject();
	/* draw the mesh */
	draw_mesh();

	glPopMatrix();
	glutSwapBuffers();
}

/*! Called when a "resize" event is received by the window. */
void reshape(int w, int h)
{
	float ar;

	win_width = w;
	win_height = h;

	ar = (float)(w) / h;
	glViewport(0, 0, w, h);               /* Set Viewport */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// magic imageing commands
	gluPerspective(40.0, /* field of view in degrees */
		ar, /* aspect ratio */
		1.0, /* Z near */
		100.0 /* Z far */);

	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

/*! helper function to remind the user about commands, hot keys */
void help()
{
	
}

/*! Keyboard call back function */
void keyBoard(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 't':
		
		break;
	case 'd':
		show_mode--;
		break;
	case 'q':
		
		break;
	case 'a':
		show_mode++;
		show_mode %= 8;
		break;
	

	case 27:
		exit(0);
		break;
	}

	glutPostRedisplay();
}

/*! mouse click call back function */
void mouseClick(int button, int state, int x, int y) {


	/* set up an arcball around the Eye's center
	switch y coordinates to right handed system  */

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		gButton = GLUT_LEFT_BUTTON;
		arcball = CArcball(win_width, win_height, x - win_width / 2, win_height - y - win_height / 2);
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		startx = x;
		starty = y;
		gButton = GLUT_MIDDLE_BUTTON;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		startx = x;
		starty = y;
		gButton = GLUT_RIGHT_BUTTON;
	}
	return;
}

/*! mouse motion call back function */
void mouseMove(int x, int y)
{
	CPoint trans;
	CQrot       rot;

	/* rotation, call arcball */
	if (gButton == GLUT_LEFT_BUTTON)
	{
		rot = arcball.update(x - win_width / 2, win_height - y - win_height / 2);
		ObjRot = rot * ObjRot;
		glutPostRedisplay();
	}

	/*xy translation */
	if (gButton == GLUT_MIDDLE_BUTTON)
	{
		double scale = 10. / win_height;
		trans = CPoint(scale * (x - startx), scale * (starty - y), 0);
		startx = x;
		starty = y;
		ObjTrans = ObjTrans + trans;
		glutPostRedisplay();
	}

	/* zoom in and out */
	if (gButton == GLUT_RIGHT_BUTTON) {
		double scale = 10. / win_height;
		trans = CPoint(0, 0, scale * (starty - y));
		startx = x;
		starty = y;
		ObjTrans = ObjTrans + trans;
		glutPostRedisplay();
	}

}

/*! setup GL states */
void setupGLstate() {

	GLfloat lightOneColor[] = { 1, 1, 1, 1 };
	GLfloat lightOneColor1[] = { 1, 1, 1, 1 };
	GLfloat globalAmb[] = { .1, .1, .1, 1 };
	GLfloat lightOnePosition[] = { .0, .0, 1, 0.0 };
	GLfloat lightOnePosition1[] = { .0, .0, -1, 0.0 };

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 0);//set the background color
	glShadeModel(GL_SMOOTH);


	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneColor);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightOneColor1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
	glLightfv(GL_LIGHT2, GL_POSITION, lightOnePosition1);

	GLfloat diffuseMaterial[4] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

}

/*! setup the texture*/
void initialize_texture(char* imagefile)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load(imagefile, &width, &height, &nrChannels, 0);

	cout <<"Channels:  "<< nrChannels << endl;

	if (data)
	{
		if (nrChannels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}			
		if (nrChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}			
		cout << "load texture finished" << endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

/*! viewer*/
void viewer(int argc, char** argv)
{	
	/* glut stuff */
	glutInit(&argc, argv);                /* Initialize GLUT */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Mesh Viewer");        /* Create window with given title */
	glViewport(0, 0, 800, 800);
	
	glutDisplayFunc(display);             /* Set-up callback functions */
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyBoard);
	setupGLstate();
	//when there is a image input
	if (argc >= 2)
	{
		initialize_texture(argv[1]);
	}

	glutMainLoop();                       /* Start GLUT event-processing loop */
			
}



#endif
