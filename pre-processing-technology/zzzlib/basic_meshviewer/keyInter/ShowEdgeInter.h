#ifndef _ShowEdge_H_
#define _ShowEdge_H_

#include<vector>
using namespace std;

#include"viewerKeyInter.h"
#include"viewerParas.h"

namespace Viewer
{

	template<typename M>
	class CShowEdgeInter :public CKeyInter
	{
	public:
		CShowEdgeInter(unsigned char _key, CViewerParas* _pParas) :CKeyInter(_key)
		{
			m_pParas = _pParas;
		}
		~CShowEdgeInter() {};
		virtual void keyInter(int _x, int _y)
		{
			m_pParas->mShowEdge = !(m_pParas->mShowEdge);
		}
		CViewerParas* m_pParas;
	};

	
}






#endif
