#include <iostream>

#include "Tool.h"
#include "ToolMesh.h"

using namespace std;
using namespace MeshLib;

void main(int argc, char **argv)
{
	CTMesh mesh;
	mesh.read_m(argv[1]);
	CTool<CTMesh> tool(&mesh);
	tool.harmonic_map_square();
	tool.quad_remesh_from_square();
	tool._change_color();

	mesh.write_m(argv[2]);
	cout << "Finished!" << endl;
}