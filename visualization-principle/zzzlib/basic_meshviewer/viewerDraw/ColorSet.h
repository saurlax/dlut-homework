#ifndef _VIEWER_COLOR_H_
#define _VIEWER_COLOR_H_

#include"../freeglut/glut.h"
#include"../freeglut/freeglut.h"

namespace ViewerColor
{

	/* set color */
	static string color_flag = "rgb";
	static GLdouble default_color[] = { 204.0 / 255.0 , 232.0 / 255.0 , 207.0 / 255.0 };
	static GLdouble color_pallete[16][3] =
	{
		{0,0,0},{1,0,0},{0,1,0},{0,0,1},{1,1,0},{1,0,1},{0,1,1},{1,1,1},
		{0.5,0.5,0.5},{1,0.5,0.5},{0.5,1,0.5},{0.5,0.5,1},{1,1,0.5},{1,0.5,1},{0.5,1,1},{1,1,1}
	};
	static GLdouble edge_color[] = { 0,0,0 };
	//GLdouble face_color[] = { 1,218.0 / 255,185.0 / 255 };
	static GLdouble face_color[] = { 145.0 / 255,137.0 / 255,135.0 / 255 };
	static GLdouble red_color[] = { 1.0,0,0 };
	static GLdouble blue_color[] = { 0,0,1.0 };
	static GLdouble yellow_color[] = { 1.0,1.0,0 };

}


#endif
