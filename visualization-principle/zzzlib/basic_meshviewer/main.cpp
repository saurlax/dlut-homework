#include <iostream>
using namespace std;
#include "ToolMesh.h"
using namespace MeshLib;
#include "viewerCore/viewer.h"
#include "viewerDraw/viewerDraw.h"
#include "viewerDraw/viewerZcolorDraw.h"
#include "viewerDraw/viewerNormalDraw.h"
#include "viewerDraw/viewerComplexFunction.h"
#include "preprocess/viewerPrepares.h"
#include "keyInter/Resize_multi_08.h"
#include "keyInter/Resize_divid_08.h"
#include "keyInter/ShowEdgeInter.h"
#include <complex>
using namespace Viewer;

static double calculateVolume(CTMesh &mesh)
{
	double volume = 0.0;

	for (CTMesh::MeshFaceIterator fit(&mesh); !fit.end(); ++fit)
	{
		CToolFace *face = *fit;

		CHalfEdge *he = (CToolHalfEdge *)face->halfedge();
		CToolVertex *v1 = (CToolVertex *)he->source();
		CToolVertex *v2 = (CToolVertex *)he->target();
		he = he->he_next();
		CToolVertex *v3 = (CToolVertex *)he->target();

		CPoint p1 = v1->point();
		CPoint p2 = v2->point();
		CPoint p3 = v3->point();

		volume += p1 * (p2 ^ p3) / 6.0;
	}

	return volume;
}

int main(int argc, char *argv[])
{
	CTMesh mesh;
	mesh.read_m(argv[1]);

	cout << "Volume: " << calculateVolume(mesh) << endl;

	PreOpers.addOper(new CPreNormal<CTMesh>(&mesh));
	
	vector<complex<double>> zeros = {
		complex<double>(0, 0),
	};
	vector<complex<double>> poles = {
		complex<double>(0.1, 0.1)
	};
	
	DrawOpers.addOper(new CComplexFunctionVisualizer<CTMesh>(&mesh, zeros, poles, 1, 1, &mViewerParas));
	DrawOpers.addOper(new CDrawVertices<CTMesh>(&mesh, &mViewerParas));
	DrawOpers.addOper(new CDrawEdges<CTMesh>(&mesh, &mViewerParas));
	
	KeyBdInter.addKeyInter(new CKeyResizeMulti08<CTMesh>('*', &mesh));
	KeyBdInter.addKeyInter(new CKeyResizeDivid08<CTMesh>('/', &mesh));
	KeyBdInter.addKeyInter(new CShowEdgeInter<CTMesh>('e', &mViewerParas));
	
	viewer(argc, argv, "checker_1k.bmp");
}
