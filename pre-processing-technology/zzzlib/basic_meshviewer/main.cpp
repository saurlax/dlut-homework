#include <iostream>
using namespace std;
/* Mesh */
#include "ToolMesh.h"
using namespace MeshLib;
/* viewer */
#include "viewerCore/viewer.h"
#include "viewerDraw/viewerDraw.h"
#include "preprocess/viewerPrepares.h"
#include "keyInter/Resize_multi_08.h"
#include "keyInter/Resize_divid_08.h"
#include "keyInter/ShowEdgeInter.h"
using namespace Viewer;

/* main  */
int main(int argc, char* argv[])
{
	CTMesh mesh;
	mesh.read_m(argv[1]);

	PreOpers.addOper(new CPreNormal<CTMesh>(&mesh));

	DrawOpers.addOper(new CDrawEdges<CTMesh>(&mesh, &mViewerParas));
	DrawOpers.addOper(new CDrawFaces<CTMesh>(&mesh,&mViewerParas));
	DrawOpers.addOper(new CDrawVertices<CTMesh>(&mesh, &mViewerParas));
	DrawOpers.addOper(new CDrawEdges<CTMesh>(&mesh, &mViewerParas));

	KeyBdInter.addKeyInter(new CKeyResizeMulti08<CTMesh>('*', &mesh));
	KeyBdInter.addKeyInter(new CKeyResizeDivid08<CTMesh>('/', &mesh));
	KeyBdInter.addKeyInter(new CShowEdgeInter<CTMesh>('e', &mViewerParas));

	viewer(argc, argv, NULL);
}