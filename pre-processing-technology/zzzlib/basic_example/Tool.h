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
		void split();

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
}
#endif