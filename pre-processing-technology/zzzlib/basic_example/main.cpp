#include <iostream>

#include "Tool.h"
#include "ToolMesh.h"

using namespace std;
using namespace MeshLib;

void main(int argc, char **argv)
{
	CTMesh mesh;
	CTool<CTMesh> tool(&mesh);
	tool.bilinear_gen(CPoint(0, 0, 1), CPoint(3, 1, 4), CPoint(-3, 1, 5), CPoint(-4, 2, 4));
	tool.harmonic_map();
	tool._change_color();

	mesh.write_m(argv[2]);
	cout << "Finished!" << endl;
}