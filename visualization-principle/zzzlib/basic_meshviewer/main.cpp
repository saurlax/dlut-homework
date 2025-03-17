#include <iostream>
using namespace std;
/* Mesh */
#include "ToolMesh.h"
using namespace MeshLib;
/* viewer */
#include "viewerCore/viewer.h"
#include "viewerDraw/viewerDraw.h"
#include "viewerDraw/viewerZcolorDraw.h"
#include "viewerDraw/viewerNormalDraw.h"
#include "preprocess/viewerPrepares.h"
#include "keyInter/Resize_multi_08.h"
#include "keyInter/Resize_divid_08.h"
#include "keyInter/ShowEdgeInter.h"
using namespace Viewer;

double calculateVolume(CTMesh &mesh)
{
	double volume = 0.0;

	// 遍历所有三角面片
	for (CTMesh::MeshFaceIterator fit(&mesh); !fit.end(); ++fit)
	{
		CToolFace *face = *fit;

		// 获取面的三个顶点（使用半边结构）
		CHalfEdge *he = (CToolHalfEdge *)face->halfedge();
		CToolVertex *v1 = (CToolVertex *)he->source();
		CToolVertex *v2 = (CToolVertex *)he->target();
		he = he->he_next();
		CToolVertex *v3 = (CToolVertex *)he->target();

		CPoint p1 = v1->point();
		CPoint p2 = v2->point();
		CPoint p3 = v3->point();

		// 计算该面片的体积贡献
		volume += p1 * (p2 ^ p3) / 6.0;
	}

	return volume;
}

/* main  */
int main(int argc, char *argv[])
{
	/* 读入文件 */
	CTMesh mesh;
	mesh.read_m(argv[1]);

	cout << "Volume: " << calculateVolume(mesh) << endl;

	/* 预处理 */
	PreOpers.addOper(new CPreNormal<CTMesh>(&mesh));
	/* 可视化 */
	DrawOpers.addOper(new CDrawEdges<CTMesh>(&mesh, &mViewerParas));
	// DrawOpers.addOper(new CDrawFaces<CTMesh>(&mesh,&mViewerParas));
	DrawOpers.addOper(new CDrawZcolorFaces<CTMesh>(&mesh, &mViewerParas));
	DrawOpers.addOper(new CDrawVertices<CTMesh>(&mesh, &mViewerParas));
	DrawOpers.addOper(new CDrawEdges<CTMesh>(&mesh, &mViewerParas));
	DrawOpers.addOper(new CDrawNormals<CTMesh>(&mesh, &mViewerParas));
	/* 键盘交互 */
	KeyBdInter.addKeyInter(new CKeyResizeMulti08<CTMesh>('*', &mesh));
	KeyBdInter.addKeyInter(new CKeyResizeDivid08<CTMesh>('/', &mesh));
	KeyBdInter.addKeyInter(new CShowEdgeInter<CTMesh>('e', &mViewerParas));
	/* OpenGL */
	viewer(argc, argv, NULL);
}
