#ifndef _VIEWER_COMPLEX_FUNCTION_H_
#define _VIEWER_COMPLEX_FUNCTION_H_

#include <vector>
#include <complex>
#include <cmath>
#include <map>
#include <limits>
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
  class CComplexFunctionVisualizer : public COper
  {
  public:
    CComplexFunctionVisualizer(
        M *_pMesh,
        vector<complex<double>> _zeros,
        vector<complex<double>> _poles,
        int _numerator = 1,
        int _denominator = 1,
        CViewerParas *_pParas = NULL) : COper()
    {
      m_pMesh = _pMesh;
      m_zeros = _zeros;
      m_poles = _poles;
      m_numerator = _numerator;
      m_denominator = _denominator;
      m_pParas = _pParas;

      if (m_denominator != 1 && m_denominator != 2 && m_denominator != 4)
      {
        m_denominator = 1;
      }

      normalizeModel();
      storeOriginalVertices();
    };
    ~CComplexFunctionVisualizer() {};

  public:
    void storeOriginalVertices()
    {
      m_originalPoints.clear();
      for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); ++viter)
      {
        M::CVertex *pV = *viter;
        m_originalPoints[pV] = pV->point();
      }
    }

    complex<double> evaluateFunction(complex<double> z)
    {
      complex<double> result(1.0, 0.0);

      for (const auto &zero : m_zeros)
      {
        result *= pow(z - zero, static_cast<double>(m_numerator) / m_denominator);
      }

      for (const auto &pole : m_poles)
      {
        result /= pow(z - pole, static_cast<double>(m_numerator) / m_denominator);
      }

      return result;
    }

    virtual void impOper()
    {
      if (m_pParas != NULL && m_pParas->mShowFace == false)
        return;

      transformVerticesByFunction();
      renderTexturedMesh();
    }

    void transformVerticesByFunction()
    {
      for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); ++viter)
      {
        M::CVertex *pV = *viter;
        CPoint originalPoint = m_originalPoints[pV];

        complex<double> z(originalPoint[0], originalPoint[1]);
        complex<double> fz = evaluateFunction(z);

        pV->point()[0] = fz.real();
        pV->point()[1] = fz.imag();
      }
    }

    void renderTexturedMesh()
    {
      glEnable(GL_POLYGON_OFFSET_FILL);
      glPolygonOffset(2.0f, 2.0f);

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, 1);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glEnable(GL_LIGHTING);
      glColor3f(1.0f, 1.0f, 1.0f);

      for (M::MeshFaceIterator fiter(m_pMesh); !fiter.end(); ++fiter)
      {
        glBegin(GL_POLYGON);
        M::CFace *pF = *fiter;
        glNormal3d(pF->normal()[0], pF->normal()[1], pF->normal()[2]);

        int vertexCount = 0;
        for (M::FaceVertexIterator fviter(pF); !fviter.end(); ++fviter, ++vertexCount)
        {
          M::CVertex *v = *fviter;
          CPoint p = v->point();

          CPoint originalP = m_originalPoints[v];
          float maxCoord = 2.0f;
          float s = (originalP[0] + maxCoord) / (2.0f * maxCoord);
          float t = (originalP[1] + maxCoord) / (2.0f * maxCoord);

          glTexCoord2f(s, t);

          glNormal3d(v->normal()[0], v->normal()[1], v->normal()[2]);
          glVertex3d(p[0], p[1], p[2]);
        }
        glEnd();
      }

      glDisable(GL_TEXTURE_2D);
    }

    void normalizeModel()
    {
      CPoint minCoord, maxCoord;

      minCoord[0] = minCoord[1] = minCoord[2] = std::numeric_limits<double>::max();
      maxCoord[0] = maxCoord[1] = maxCoord[2] = -std::numeric_limits<double>::max();

      for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); ++viter)
      {
        M::CVertex *pV = *viter;
        CPoint p = pV->point();

        for (int i = 0; i < 3; i++)
        {
          if (p[i] < minCoord[i])
            minCoord[i] = p[i];
          if (p[i] > maxCoord[i])
            maxCoord[i] = p[i];
        }
      }

      CPoint center = (minCoord + maxCoord) * 0.5;
      double maxRange = 0;
      for (int i = 0; i < 3; i++)
      {
        double range = maxCoord[i] - minCoord[i];
        if (range > maxRange)
          maxRange = range;
      }

      double scaleFactor = 2.0 / maxRange;

      for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); ++viter)
      {
        M::CVertex *pV = *viter;
        CPoint &p = pV->point();

        p = p - center;
        p = p * scaleFactor;
      }
    }

  protected:
    M *m_pMesh;
    vector<complex<double>> m_zeros;
    vector<complex<double>> m_poles;
    int m_numerator;
    int m_denominator;
    CViewerParas *m_pParas;
    map<typename M::CVertex *, CPoint> m_originalPoints;
  };
}

#endif
