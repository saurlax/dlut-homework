#ifndef _VIEWER_ZColor_DRAW_H_
#define _VIEWER_ZColor_DRAW_H_

#include<vector>
using namespace std;

#include"../freeglut/glut.h"
#include"../freeglut/freeglut.h"
#include"ImpOperS.h"
#include"viewerParas.h"
using namespace Viewer;
#include"ColorSet.h"
using namespace ViewerColor;

namespace Viewer
{
	
	template<typename M>
	class CDrawZcolorFaces : public COper
	{
	public:
		CDrawZcolorFaces(typename M* _pMesh,CViewerParas* _pParas=NULL) : COper()
		{
			m_pMesh = _pMesh;
			m_pParas = _pParas;
		};
		~CDrawZcolorFaces() {};
	public:
		virtual void impOper()
		{
			

			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(2.0f, 2.0f);
			glEnable(GL_LIGHTING);
			//glColor3f(135.0 / 255.0f, 206.0 / 255.0f, 250.0 / 255.0f); 
			
			for (M::MeshFaceIterator fiter(m_pMesh); !fiter.end(); ++fiter)
			{
				glBegin(GL_POLYGON);
				M::CFace* pF = *fiter;
				glNormal3d(pF->normal()[0], pF->normal()[1], pF->normal()[2]);
				for (M::FaceVertexIterator fviter(pF); !fviter.end(); ++fviter)
				{
					glColor3f(-(*fviter)->point()[2]/2+1, 0, (*fviter)->point()[2]/2+1);
					//glNormal3d((*fviter)->normal()[0], (*fviter)->normal()[1], (*fviter)->normal()[2]);
					glVertex3d((*fviter)->point()[0], (*fviter)->point()[1], (*fviter)->point()[2]);
					
				}
				glEnd();
			}
		}
	protected:
		typename M* m_pMesh;
		CViewerParas* m_pParas;
	};	

}






#endif
