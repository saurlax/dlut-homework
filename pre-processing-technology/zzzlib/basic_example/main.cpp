#include <iostream>

#include "Tool.h"
#include "ToolMesh.h"
#include "TriTopOper.h"

using namespace std;
using namespace MeshLib;

void main(int argc, char **argv)
{
	CTMesh mesh;
	CTool<CTMesh> tool(&mesh);
	CTriTopOper<CTMesh> topOper(&mesh);

	CToolVertex *v1 = mesh.createVertex(1);
	CToolVertex *v2 = mesh.createVertex(2);
	CToolVertex *v3 = mesh.createVertex(3);
	CToolVertex *v4 = mesh.createVertex(4);

	v1->point() = CPoint(0.0, 0.0, 0.0);
	v2->point() = CPoint(0.5, 0.0, 0.0);
	v3->point() = CPoint(0.0, 0.5, 0.0);
	v4->point() = CPoint(1.0, 1.0, 0.0);

	CToolVertex *f1[3] = {v1, v2, v3};
	CToolVertex *f2[3] = {v2, v4, v3};

	mesh.createFace(f1, 1);
	mesh.createFace(f2, 2);

	tool._change_color();
	mesh.write_m("before_swap.m");

	CTMesh::CEdge *e1 = mesh.vertexEdge(v2, v3);
	topOper.swapEdge(e1);

	mesh.write_m("after_swap.m");
	cout << "Finished!" << endl;
}