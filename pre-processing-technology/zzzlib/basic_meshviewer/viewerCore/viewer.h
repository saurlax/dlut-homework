#ifndef _VIEWER_H_
#define _VIEWER_H_

#include<math.h>
#include<vector>
using namespace std;

#include"../../freeglut/glut.h"
#include"../../freeglut/freeglut.h"
#include"Arcball.h"
#include"../../MeshLib/Geometry/Point.h"
#include"quat.h"
#include"stb_image_header.h"

#include"../keyInter/viewerKeyInter.h"
#include"../ImpOperS.h"
using namespace AlgoFrame;

#include"../viewerParas.h"
using namespace Viewer;

namespace Viewer
{
	/* for show */
	CImpOperS PreOpers;
	CImpOperS DrawOpers;
	CKeyBdInter KeyBdInter;
	CViewerParas mViewerParas;

	/* window width and height */
	int win_width, win_height;//record the current size of the window
	int gButton;//record the current mouse button 
	int startx, starty;//record the postion when the button was pushed down

	/* rotation quaternion and translation vector for the object */
	CQrot       ObjRot(0, 0, 1, 0);
	CPoint      ObjTrans(0, 0, 0);

	/* arcball object */
	CArcball arcball;

	/*! setup the object, transform from the world to the object coordinate system */
	void setupObject(void)
	{
		double rot[16];

		glTranslated(ObjTrans[0], ObjTrans[1], ObjTrans[2]);
		ObjRot.convert(rot);

		glMultMatrixd((GLdouble*)rot);
	}

	/* set light */
	void setupLight()
	{
		GLfloat light1Position[4] = { 0, 0, 5, 0 };
		GLfloat light2Position[4] = { 0, 0, -5, 0 };
		glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
		glLightfv(GL_LIGHT2, GL_POSITION, light2Position);
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
		glClearColor(1, 1, 1, 0);//set the background color
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

	/*! the eye is always fixed at world z = +5 */
	void setupEye(void) {
		glLoadIdentity();//reset the trans matrix to indentity matrix
		gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);//we are facing the z direction and up direction is y axis
	}

	/*! draw mesh */
	void draw_mesh()
	{	
		DrawOpers.impOperS();
	}

	/*! display call back function*/
	void display()
	{
		/* clear frame buffer */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setupLight();
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
		KeyBdInter.keyBdInter(key,x,y);
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
		CQrot rot;

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

		cout << "Channels:  " << nrChannels << endl;

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
	void viewer(int argc, char** argv, char* _texturefile)
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
		//initialize
		if (_texturefile != NULL)
		{
			initialize_texture(_texturefile);
		}
		//prepare
		PreOpers.impOperS();
		//rendering
		glutMainLoop();                       /* Start GLUT event-processing loop */

	}


}


#endif
