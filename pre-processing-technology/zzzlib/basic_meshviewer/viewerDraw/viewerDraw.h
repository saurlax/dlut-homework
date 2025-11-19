#ifndef _VIEWER_DRAW_H_
#define _VIEWER_DRAW_H_

#include <vector>
using namespace std;

#include "../freeglut/glut.h"
#include "../freeglut/freeglut.h"
#include "ImpOperS.h"
#include "viewerParas.h"
using namespace Viewer;
#include "ColorSet.h"
using namespace ViewerColor;

namespace Viewer
{
	template <typename M>
	class CDrawVertices : public COper
	{
	public:
		CDrawVertices(typename M *_pMesh, CViewerParas *_pParas = NULL) : COper()
		{
			m_pMesh = _pMesh;
			m_pParas = _pParas;
		};
		~CDrawVertices() {};

	public:
		virtual void impOper()
		{
			if (m_pParas != NULL && m_pParas->mShowVertex == false)
				return;

			glEnable(GL_LIGHTING);
			glPointSize(1.0);
			glBegin(GL_POINTS);
			for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); ++viter)
			{
				M::CVertex *pV = *viter;
				CPoint p = pV->point();
				/* 检索是否有颜色 */
				if (pV->string().find(color_flag) != -1)
				{
					CPoint color = pV->rgb();
					glColor3f(color[0], color[1], color[2]);
				}
				else
				{
					glColor3f(default_color[0], default_color[1], default_color[2]);
				}
				glVertex3d(p[0], p[1], p[2]);
			}
			glEnd();
		}

	protected:
		typename M *m_pMesh;
		CViewerParas *m_pParas;
	};

	template <typename M>
	class CDrawEdges : public COper
	{
	public:
		CDrawEdges(typename M *_pMesh, CViewerParas *_pParas = NULL) : COper()
		{
			m_pMesh = _pMesh;
			m_pParas = _pParas;
		};
		~CDrawEdges() {};

	public:
		virtual void impOper()
		{
			if (m_pParas != NULL &&
					m_pParas->mShowEdge == false)
				return;

			glDisable(GL_LIGHTING);
			glLineWidth(0.6f);
			glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 128.0f / 255.0f);
			glBegin(GL_LINES);
			for (M::MeshEdgeIterator eiter(m_pMesh); !eiter.end(); ++eiter)
			{
				M::CEdge *pE = *eiter;
				glVertex3f(pE->halfedge(0)->source()->point()[0],
									 pE->halfedge(0)->source()->point()[1],
									 pE->halfedge(0)->source()->point()[2]);
				glVertex3f(pE->halfedge(0)->target()->point()[0],
									 pE->halfedge(0)->target()->point()[1],
									 pE->halfedge(0)->target()->point()[2]);
			}
			glEnd();
		}

	protected:
		typename M *m_pMesh;
		CViewerParas *m_pParas;
	};

	template <typename M>
	class CDrawFaces : public COper
	{
	public:
		CDrawFaces(typename M *_pMesh, CViewerParas *_pParas = NULL) : COper()
		{
			m_pMesh = _pMesh;
			m_pParas = _pParas;
		};
		~CDrawFaces() {};

	public:
		virtual void impOper()
		{
			if (m_pParas != NULL && m_pParas->mShowFace == false)
				return;

			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(2.0f, 2.0f);
			glEnable(GL_LIGHTING);

			std::srand(std::time(nullptr));
			glColor3f(235.0 / 255.0f, 206.0 / 255.0f, 250.0 / 255.0f);
			for (M::MeshFaceIterator fiter(m_pMesh); !fiter.end(); ++fiter)
			{
				glBegin(GL_POLYGON);
				M::CFace *pF = *fiter;
				glNormal3d(pF->normal()[0], pF->normal()[1], pF->normal()[2]);
				for (M::FaceVertexIterator fviter(pF); !fviter.end(); ++fviter)
				{
					glNormal3d((*fviter)->normal()[0], (*fviter)->normal()[1], (*fviter)->normal()[2]);
					glVertex3d((*fviter)->point()[0], (*fviter)->point()[1], (*fviter)->point()[2]);
				}
				glEnd();
			}
		}

	protected:
		typename M *m_pMesh;
		CViewerParas *m_pParas;
	};

}

#endif
