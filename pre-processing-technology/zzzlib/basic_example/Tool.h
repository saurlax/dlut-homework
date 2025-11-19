#ifndef _TOOL_H_
#define _TOOL_H_

#include<vector>
#include<math.h>

#include "ToolMesh.h"

#ifndef PI
#define PI 3.1415926535
#endif

namespace MeshLib
{
	using namespace std;

	template<typename M>
	class CTool
	{
	public:
		CTool(M* pMesh);
		~CTool(){};

		void test();
		void _change_color();
	protected:
		typename M* m_pMesh;
	};

	template<typename M>
	CTool<M>::CTool(M* pMesh)
	{
		m_pMesh = pMesh;
	}

	template<typename M>
	void CTool<M>::test()
	{
		cout << "mesh vertex num: " << m_pMesh->numVertices() << endl;

	}

	template<typename M>
	void CTool<M>::_change_color()
	{
		for (M::MeshVertexIterator mv(m_pMesh); !mv.end(); mv++)
		{
			M::CVertex* pVertex = mv.value();
			pVertex->rgb()[0] = 1;
			pVertex->rgb()[1] = 1;
			pVertex->rgb()[2] = 0;
		}
	}
	
}
#endif