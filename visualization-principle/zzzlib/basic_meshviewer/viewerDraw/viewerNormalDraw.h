#ifndef _VIEWER_NORMAL_DRAW_H_
#define _VIEWER_NORMAL_DRAW_H_

#include <vector>
using namespace std;

#include "../MeshLib/Geometry/Point.h"
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
  class CDrawNormals : public COper
  {
  public:
    CDrawNormals(typename M *_pMesh, CViewerParas *_pParas = NULL) : COper()
    {
      m_pMesh = _pMesh;
      m_pParas = _pParas;
    };
    ~CDrawNormals() {};

  public:
    virtual void impOper()
    {
      if (m_pParas != NULL &&
          m_pParas->mShowEdge == false)
        return;

      glDisable(GL_LIGHTING);
      glLineWidth(0.6f);
      glColor3f(128.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
      glBegin(GL_LINES);
      for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); ++viter)
      {
        M::CVertex *pV = *viter;
        CPoint p = pV->point();
        CPoint n = pV->normal() * 10;
        glVertex3d(p[0], p[1], p[2]);
        glVertex3d(p[0] + n[0], p[1] + n[1], p[2] + n[2]);
      }
      glEnd();
    }

  protected:
    typename M *m_pMesh;
    CViewerParas *m_pParas;
  };

}

#endif
