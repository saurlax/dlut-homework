#ifndef _RESIZE_DIVID_08_H_
#define _RESIZE_DIVID_08_H_

#include<vector>
using namespace std;

#include"viewerKeyInter.h"

namespace Viewer
{

	template<typename M>
	class CKeyResizeDivid08 :public CKeyInter
	{
	public:
		CKeyResizeDivid08(unsigned char _key, typename M* _pMesh) :CKeyInter(_key)
		{
			m_pMesh = _pMesh;
		}
		~CKeyResizeDivid08() {};
		virtual void keyInter(int _x, int _y)
		{
			cout << "resize the model by multiply all points with 1.25" << endl;
			for (M::MeshVertexIterator mv(m_pMesh); !mv.end(); mv++)
			{
				M::CVertex* pVertex = mv.value();
				pVertex->point() *= 1.25;
			}
		}
		typename M* m_pMesh;
	};

	
}






#endif
