#ifndef _RESIZE_MULTI_08_H_
#define _RESIZE_MULTI_08_H_

#include<vector>
using namespace std;

#include"viewerKeyInter.h"

namespace Viewer
{

	template<typename M>
	class CKeyResizeMulti08 :public CKeyInter
	{
	public:
		CKeyResizeMulti08(unsigned char _key, typename M* _pMesh) :CKeyInter(_key)
		{
			m_pMesh = _pMesh;
		}
		~CKeyResizeMulti08() {};
		virtual void keyInter(int _x, int _y)
		{
			cout << "resize the model by multiply all points with 0.8" << endl;
			for (M::MeshVertexIterator mv(m_pMesh); !mv.end(); mv++)
			{
				M::CVertex* pVertex = mv.value();
				pVertex->point() *= 0.8;
			}
		}
		typename M* m_pMesh;
	};

	
}






#endif
