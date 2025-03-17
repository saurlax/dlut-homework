#include<iostream>

#include"Tool.h"
#include"ToolMesh.h"

using namespace std;
using namespace MeshLib;


void main(int argc, char** argv)
{
	CTMesh mesh;
	mesh.read_m(argv[1]);
	CTool<CTMesh> tool(&mesh);
	tool.test();
	tool._change_color();
	mesh.write_m(argv[2]);

	cout << "finished !!! press any key to continue!!!" << endl;
	getchar();



}