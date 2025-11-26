#ifndef _TOOL_H_
#define _TOOL_H_

#include <vector>
#include <math.h>

#include "ToolMesh.h"
#include "TriTopOper.h"

#ifndef PI
#define PI 3.1415926535
#endif

namespace MeshLib
{
	using namespace std;

	template <typename M>
	class CTool
	{
	public:
		CTool(M *pMesh);
		~CTool() {};

		void test();
		void _change_color();
		void split();
		void bilinear_gen(CPoint p1, CPoint p2, CPoint p3, CPoint p4);

	protected:
		typename M *m_pMesh;
	};

	template <typename M>
	CTool<M>::CTool(M *pMesh)
	{
		m_pMesh = pMesh;
	}

	template <typename M>
	void CTool<M>::test()
	{
		cout << "mesh vertex num: " << m_pMesh->numVertices() << endl;
	}

	template <typename M>
	void CTool<M>::_change_color()
	{
		for (M::MeshVertexIterator mv(m_pMesh); !mv.end(); mv++)
		{
			M::CVertex *pVertex = mv.value();
			pVertex->rgb()[0] = 1;
			pVertex->rgb()[1] = 1;
			pVertex->rgb()[2] = 0;
		}
	}

	template <typename M>
	void CTool<M>::split()
	{
		vector<M::CFace *> faces;
		for (M::MeshFaceIterator mf(m_pMesh); !mf.end(); mf++)
		{
			M::CFace *pFace = mf.value();
			faces.push_back(pFace);
		}
		CTriTopOper<M> triOper(m_pMesh);
		for (auto face : faces)
		{
			M::CFace *pFace = face;
			cout << "spliting face id: " << pFace->id() << endl;
			CPoint p = (pFace->halfedge()->source()->point() +
									pFace->halfedge()->he_next()->source()->point() +
									pFace->halfedge()->he_next()->he_next()->source()->point()) /
								 3.0;
			M::CVertex *pV = triOper.splitFace(pFace);
			pV->point() = p;
		}
	}

	template <typename M>
	void CTool<M>::bilinear_gen(CPoint p1, CPoint p2, CPoint p3, CPoint p4)
	{
		// R(u,v) = (1-v)((1-u)P1+uP2)+v((1-u)P4+uP3)
		int div = 10;
		CTriTopOper<M> triOper(m_pMesh);
		M::CVertex *vtxs[11][11];
		for (int i = 0; i <= div; i++)
		{
			double u = double(i) / double(div);
			for (int j = 0; j <= div; j++)
			{
				double v = double(j) / double(div);
				CPoint p = (p1 * (1 - u) + p2 * u) * (1 - v) + (p4 * (1 - u) + p3 * u) * v;
				M::CVertex *pV = m_pMesh->createVertex(m_pMesh->numVertices() + 1);
				pV->point() = p;
				vtxs[i][j] = pV;
			}
		}
		for (int i = 0; i < div; i++)
		{
			for (int j = 0; j < div; j++)
			{
				M::CVertex *fv1[3] = {vtxs[i][j], vtxs[i + 1][j], vtxs[i + 1][j + 1]};
				m_pMesh->createFace(fv1, m_pMesh->numFaces() + 1);
				M::CVertex *fv2[3] = {vtxs[i][j], vtxs[i + 1][j + 1], vtxs[i][j + 1]};
				m_pMesh->createFace(fv2, m_pMesh->numFaces() + 1);
			}
		}
	}
}
#endif