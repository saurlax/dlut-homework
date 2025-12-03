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
		void _change_color();
		void split();
		void bilinear_gen(CPoint p1, CPoint p2, CPoint p3, CPoint p4);
		void harmonic_map(int max_iter = 5000, double tol = 1e-6);
		void harmonic_map_square(int max_iter = 5000, double tol = 1e-6);

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
	void CTool<M>::_change_color()
	{
		for (M::MeshVertexIterator mv(m_pMesh); !mv.end(); mv++)
		{
			M::CVertex *pVertex = mv.value();
			pVertex->rgb() = {1, 0.8, 0.8};
		}
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

	template <typename M>
	void CTool<M>::bilinear_gen(CPoint p1, CPoint p2, CPoint p3, CPoint p4)
	{
		// R(u,v) = (1-v)((1-u)P1+uP2)+v((1-u)P4+uP3)
		int div = 10;
		CTriTopOper<M> triOper(m_pMesh);
		M::CVertex *vtxs[11][11];
		for (int i = 0; i <= div; i++)
		{
			double u = double(i) / double(div);
			for (int j = 0; j <= div; j++)
			{
				double v = double(j) / double(div);
				CPoint p = (p1 * (1 - u) + p2 * u) * (1 - v) + (p4 * (1 - u) + p3 * u) * v;
				M::CVertex *pV = m_pMesh->createVertex(m_pMesh->numVertices() + 1);
				pV->point() = p;
				vtxs[i][j] = pV;
			}
		}
		for (int i = 0; i < div; i++)
		{
			for (int j = 0; j < div; j++)
			{
				M::CVertex *fv1[3] = {vtxs[i][j], vtxs[i + 1][j], vtxs[i + 1][j + 1]};
				m_pMesh->createFace(fv1, m_pMesh->numFaces() + 1);
				M::CVertex *fv2[3] = {vtxs[i][j], vtxs[i + 1][j + 1], vtxs[i][j + 1]};
				m_pMesh->createFace(fv2, m_pMesh->numFaces() + 1);
			}
		}
		m_pMesh->labelBoundary();
	}

	template <typename M>
	void CTool<M>::harmonic_map(int max_iter, double tol)
	{
		M::CBoundary boundary(m_pMesh);
		if (boundary.loops().empty())
		{
			std::cerr << "Harmonic map requires an open mesh with boundary." << std::endl;
			return;
		}

		// 取长度最大的边界环并按弧长映射到单位圆
		auto *loop = boundary.loops()[0];
		std::vector<typename M::CHalfEdge *> hes;
		hes.reserve(loop->halfedges().size());
		for (auto he : loop->halfedges())
		{
			hes.push_back(he);
		}
		if (hes.size() < 3)
		{
			std::cerr << "Boundary too small for mapping." << std::endl;
			return;
		}

		// 收集边界顶点顺序及边长
		std::vector<typename M::CVertex *> bverts;
		bverts.reserve(hes.size());
		double perimeter = 0.0;
		std::vector<double> seg_len(hes.size(), 0.0);
		for (size_t i = 0; i < hes.size(); ++i)
		{
			auto he = hes[i];
			auto vS = m_pMesh->halfedgeSource(he);
			auto e = m_pMesh->halfedgeEdge(he);
			double el = m_pMesh->edgeLength(e);
			seg_len[i] = el;
			perimeter += el;
			bverts.push_back(vS);
		}

		if (perimeter <= 1e-12)
		{
			std::cerr << "Boundary perimeter is zero." << std::endl;
			return;
		}

		// 边界映射到单位圆，直接修改3D坐标 (x,y,z) = (cos(t), sin(t), 0)
		double acc = 0.0;
		for (size_t i = 0; i < bverts.size(); ++i)
		{
			if (i > 0)
				acc += seg_len[i - 1];
			double t = (acc / perimeter) * 2.0 * PI;
			double cx = cos(t);
			double cy = sin(t);
			bverts[i]->point() = CPoint(cx, cy, 0.0);
			bverts[i]->boundary() = true;
		}

		// 初始化内点3D坐标为(0,0,0)
		std::vector<typename M::CVertex *> interior_vertices;
		for (typename M::MeshVertexIterator mv(m_pMesh); !mv.end(); mv++)
		{
			auto v = mv.value();
			if (!v->boundary())
			{
				v->point() = CPoint(0.0, 0.0, 0.0);
				interior_vertices.push_back(v);
			}
		}

		if (interior_vertices.empty())
		{
			return;
		}

		// 图拉普拉斯迭代：对x,y坐标分别求解调和方程（z始终为0）
		std::vector<CPoint> next_pos(interior_vertices.size());
		for (int it = 0; it < max_iter; ++it)
		{
			double max_delta = 0.0;
			for (size_t idx = 0; idx < interior_vertices.size(); ++idx)
			{
				auto v = interior_vertices[idx];
				double sum_x = 0.0, sum_y = 0.0;
				int deg = 0;
				for (typename M::VertexVertexIterator vv(v); !vv.end(); vv++)
				{
					auto nb = vv.value();
					sum_x += nb->point()[0];
					sum_y += nb->point()[1];
					++deg;
				}
				if (deg > 0)
				{
					sum_x /= (double)deg;
					sum_y /= (double)deg;
				}
				next_pos[idx] = CPoint(sum_x, sum_y, 0.0);
			}

			// 应用更新并计算收敛性
			for (size_t idx = 0; idx < interior_vertices.size(); ++idx)
			{
				auto v = interior_vertices[idx];
				CPoint old_pos = v->point();
				CPoint new_pos = next_pos[idx];
				double dx = new_pos[0] - old_pos[0];
				double dy = new_pos[1] - old_pos[1];
				double delta = sqrt(dx * dx + dy * dy);
				if (delta > max_delta)
					max_delta = delta;
				v->point() = new_pos;
			}

			if (max_delta < tol)
			{
				std::cout << "Harmonic map converged at iteration " << it << std::endl;
				break;
			}
		}
	}

	template <typename M>
	void CTool<M>::harmonic_map_square(int max_iter, double tol)
	{
		M::CBoundary boundary(m_pMesh);
		if (boundary.loops().empty())
		{
			std::cerr << "Harmonic map square requires an open mesh with boundary." << std::endl;
			return;
		}

		// 取长度最大的边界环并按弧长映射到正方形
		auto *loop = boundary.loops()[0];
		std::vector<typename M::CHalfEdge *> hes;
		hes.reserve(loop->halfedges().size());
		for (auto he : loop->halfedges())
		{
			hes.push_back(he);
		}
		if (hes.size() < 4)
		{
			std::cerr << "Boundary too small for mapping." << std::endl;
			return;
		}

		// 收集边界顶点顺序及边长
		std::vector<typename M::CVertex *> bverts;
		bverts.reserve(hes.size());
		double perimeter = 0.0;
		std::vector<double> seg_len(hes.size(), 0.0);
		for (size_t i = 0; i < hes.size(); ++i)
		{
			auto he = hes[i];
			auto vS = m_pMesh->halfedgeSource(he);
			auto e = m_pMesh->halfedgeEdge(he);
			double el = m_pMesh->edgeLength(e);
			seg_len[i] = el;
			perimeter += el;
			bverts.push_back(vS);
		}

		if (perimeter <= 1e-12)
		{
			std::cerr << "Boundary perimeter is zero." << std::endl;
			return;
		}

		// 边界映射到单位正方形 [-1,1]×[-1,1]
		// 将周长均分为4段，分别对应正方形的4条边
		double side_length = perimeter / 4.0;
		double acc = 0.0;
		for (size_t i = 0; i < bverts.size(); ++i)
		{
			if (i > 0)
				acc += seg_len[i - 1];
			
			double t = acc / perimeter; // 归一化位置 [0,1)
			double x, y;
			
			// 将 [0,1) 的位置映射到正方形的4条边
			if (t < 0.25) // 底边: 从(-1,-1)到(1,-1)
			{
				double s = t / 0.25;
				x = -1.0 + 2.0 * s;
				y = -1.0;
			}
			else if (t < 0.5) // 右边: 从(1,-1)到(1,1)
			{
				double s = (t - 0.25) / 0.25;
				x = 1.0;
				y = -1.0 + 2.0 * s;
			}
			else if (t < 0.75) // 上边: 从(1,1)到(-1,1)
			{
				double s = (t - 0.5) / 0.25;
				x = 1.0 - 2.0 * s;
				y = 1.0;
			}
			else // 左边: 从(-1,1)到(-1,-1)
			{
				double s = (t - 0.75) / 0.25;
				x = -1.0;
				y = 1.0 - 2.0 * s;
			}
			
			bverts[i]->point() = CPoint(x, y, 0.0);
			bverts[i]->boundary() = true;
		}

		// 初始化内点3D坐标为(0,0,0)
		std::vector<typename M::CVertex *> interior_vertices;
		for (typename M::MeshVertexIterator mv(m_pMesh); !mv.end(); mv++)
		{
			auto v = mv.value();
			if (!v->boundary())
			{
				v->point() = CPoint(0.0, 0.0, 0.0);
				interior_vertices.push_back(v);
			}
		}

		if (interior_vertices.empty())
		{
			return;
		}

		// 图拉普拉斯迭代：对x,y坐标分别求解调和方程（z始终为0）
		std::vector<CPoint> next_pos(interior_vertices.size());
		for (int it = 0; it < max_iter; ++it)
		{
			double max_delta = 0.0;
			for (size_t idx = 0; idx < interior_vertices.size(); ++idx)
			{
				auto v = interior_vertices[idx];
				double sum_x = 0.0, sum_y = 0.0;
				int deg = 0;
				for (typename M::VertexVertexIterator vv(v); !vv.end(); vv++)
				{
					auto nb = vv.value();
					sum_x += nb->point()[0];
					sum_y += nb->point()[1];
					++deg;
				}
				if (deg > 0)
				{
					sum_x /= (double)deg;
					sum_y /= (double)deg;
				}
				next_pos[idx] = CPoint(sum_x, sum_y, 0.0);
			}

			// 应用更新并计算收敛性
			for (size_t idx = 0; idx < interior_vertices.size(); ++idx)
			{
				auto v = interior_vertices[idx];
				CPoint old_pos = v->point();
				CPoint new_pos = next_pos[idx];
				double dx = new_pos[0] - old_pos[0];
				double dy = new_pos[1] - old_pos[1];
				double delta = sqrt(dx * dx + dy * dy);
				if (delta > max_delta)
					max_delta = delta;
				v->point() = new_pos;
			}

			if (max_delta < tol)
			{
				std::cout << "Harmonic map square converged at iteration " << it << std::endl;
				break;
			}
		}
	}
}
#endif