#ifndef _VIEWER_PREPARES_H_
#define _VIEWER_PREPARES_H_

#include"../viewerParas.h"
using namespace Viewer;
#include"../ImpOperS.h"
using namespace AlgoFrame;
#include"../attrCompute/MeshNormal.h"
using namespace MeshLib;

namespace Viewer
{
	
	template<typename M>
	class CPreNormal : public COper
	{
	public:
		CPreNormal(typename M* _pMesh,CViewerParas* _pParas=NULL) :COper()
		{
			pNormal = new CMeshNormal<M>(_pMesh);
			m_pParas = _pParas;
		}
		~CPreNormal() {};
	public:
		virtual void impOper()
		{
			if (m_pParas != NULL && m_pParas->computeNormal == true)
			{
				pNormal->_face_normal();
				pNormal->_vertex_face_normal();
				m_pParas->computeNormal = false;
			}
			if (m_pParas == NULL)
			{
				pNormal->_face_normal();
				pNormal->_vertex_face_normal();
			}

		}
	protected:
		CMeshNormal<M>* pNormal;
		CViewerParas* m_pParas;
	};

		
}






#endif
