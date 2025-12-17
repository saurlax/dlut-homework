#ifndef _TRI_TOP_OPER_H_
#define _TRI_TOP_OPER_H_

#include <vector>
#include <map>
using namespace std;

namespace MeshLib
{
	template <typename M>
	class CTriTopOper
	{
	public:
		CTriTopOper(M *_mesh);
		~CTriTopOper();

	public:
		/* remesh operation */
		typename M::CVertex *splitFace(typename M::CFace *pFace);
		typename M::CVertex *splitEdge(typename M::CEdge *pEdge);
		typename M::CVertex *addVertexFaceBoundary(typename M::CHalfEdge *phe);
		typename M::CHalfEdge *addFace2Boundary(typename M::CHalfEdge *phe);
		void swapEdge(typename M::CEdge *pEdge);
		void generalEdgeSwap(typename M::CEdge *edge, typename M::CVertex *v1, typename M::CVertex *v2);

	protected:
		void _update_vertex_edges(typename M::CVertex *pVertex);
		void _attach_halfedge_to_edge(typename M::CHalfEdge *he0, typename M::CHalfEdge *he1, typename M::CEdge *e);

	protected:
		M *m_pMesh;
		/* the max id */
		int m_vertex_id;
		int m_face_id;
	};

	template <typename M>
	CTriTopOper<M>::CTriTopOper(M *_mesh)
	{
		m_pMesh = _mesh;
		// compute vertex_id and face_id
		m_vertex_id = 0;
		for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); viter++)
		{
			if (viter.value()->id() > m_vertex_id)
				m_vertex_id = viter.value()->id();
		}
		m_face_id = 0;
		for (M::MeshFaceIterator fiter(m_pMesh); !fiter.end(); fiter++)
		{
			if (fiter.value()->id() > m_face_id)
				m_face_id = fiter.value()->id();
		}
	}

	template <typename M>
	CTriTopOper<M>::~CTriTopOper()
	{
	}

	template <typename M>
	void CTriTopOper<M>::_attach_halfedge_to_edge(typename M::CHalfEdge *he0, typename M::CHalfEdge *he1, typename M::CEdge *e)
	{
		if (he0 == NULL)
		{
			e->halfedge(0) = he1;
			e->halfedge(1) = NULL;
		}
		else if (he1 == NULL)
		{
			e->halfedge(0) = he0;
			e->halfedge(1) = NULL;
		}
		else
		{
			e->halfedge(0) = he0;
			e->halfedge(1) = he1;
		}

		if (he0 != NULL)
			he0->edge() = e;
		if (he1 != NULL)
			he1->edge() = e;
	}

	template <typename M>
	typename M::CVertex *CTriTopOper<M>::splitFace(typename M::CFace *pFace)
	{
		M::CVertex *pV = m_pMesh->createVertex(++m_vertex_id);

		M::CVertex *v[3];
		M::CHalfEdge *h[3];
		M::CHalfEdge *hs[3];

		M::CEdge *eg[3];

		h[0] = m_pMesh->faceHalfedge(pFace);
		h[1] = m_pMesh->faceNextCcwHalfEdge(h[0]);
		h[2] = m_pMesh->faceNextCcwHalfEdge(h[1]);

		for (int i = 0; i < 3; i++)
		{
			v[i] = m_pMesh->halfedgeTarget(h[i]);
			eg[i] = m_pMesh->halfedgeEdge(h[i]);
			hs[i] = m_pMesh->halfedgeSym(h[i]);
		}

		// create two new faces using createFace so list/map are updated consistently
		M::CVertex *fv1[3] = {v[0], v[1], pV};
		M::CFace *f = m_pMesh->createFace(fv1, ++m_face_id);
		M::CHalfEdge *hes[3];
		hes[0] = m_pMesh->faceHalfedge(f);
		hes[1] = m_pMesh->faceNextCcwHalfEdge(hes[0]);
		hes[2] = m_pMesh->faceNextCcwHalfEdge(hes[1]);

		M::CVertex *fv2[3] = {pV, v[1], v[2]};
		f = m_pMesh->createFace(fv2, ++m_face_id);
		M::CHalfEdge *hes2[3];
		hes2[0] = m_pMesh->faceHalfedge(f);
		hes2[1] = m_pMesh->faceNextCcwHalfEdge(hes2[0]);
		hes2[2] = m_pMesh->faceNextCcwHalfEdge(hes2[1]);

		// attach halfedges that must be connected to existing edges
		_attach_halfedge_to_edge(h[0], hs[0], eg[0]);
		_attach_halfedge_to_edge(hes[1], hs[1], eg[1]);
		_attach_halfedge_to_edge(hes2[2], hs[2], eg[2]);

		pV->halfedge() = h[1];

		h[1]->vertex() = pV;
		h[2]->vertex() = v[2];

		hes[0]->vertex() = v[0];
		hes[1]->vertex() = v[1];
		hes[2]->vertex() = pV;

		hes2[0]->vertex() = pV;
		hes2[1]->vertex() = v[1];
		hes2[2]->vertex() = v[2];

		v[0]->halfedge() = h[0];
		v[1]->halfedge() = hes[1];
		v[2]->halfedge() = hes2[2];
		/*
		for( int i = 0; i < 3; i ++ )
		{
		v[i]->halfedge() = hs[i]->he_sym();
		}
		*/
		return pV;
	}

	template <typename M>
	typename M::CVertex *CTriTopOper<M>::splitEdge(typename M::CEdge *pEdge)
	{
		M::CVertex *pV = m_pMesh->createVertex(++m_vertex_id);

		M::CHalfEdge *h[12];
		M::CHalfEdge *s[6];
		M::CVertex *v[6];
		M::CEdge *eg[6];

		h[0] = m_pMesh->edgeHalfedge(pEdge, 0);
		h[1] = m_pMesh->faceNextCcwHalfEdge(h[0]);
		h[2] = m_pMesh->faceNextCcwHalfEdge(h[1]);

		h[3] = m_pMesh->edgeHalfedge(pEdge, 1);
		h[4] = m_pMesh->faceNextCcwHalfEdge(h[3]);
		h[5] = m_pMesh->faceNextCcwHalfEdge(h[4]);

		M::CFace *f[4];
		f[0] = m_pMesh->halfedgeFace(h[0]);
		f[1] = m_pMesh->halfedgeFace(h[3]);

		for (int ii = 0; ii < 6; ii++)
		{
			eg[ii] = m_pMesh->halfedgeEdge(h[ii]);
			v[ii] = m_pMesh->halfedgeVertex(h[ii]);
			s[ii] = m_pMesh->halfedgeSym(h[ii]);
		}

		// create two new faces using createFace so mesh lists/maps and edges are handled
		M::CVertex *fv2[3] = {v[1], v[2], pV};
		f[2] = m_pMesh->createFace(fv2, ++m_face_id);
		// new halfedges h[6], h[7], h[8]
		h[6] = m_pMesh->faceHalfedge(f[2]);
		h[7] = m_pMesh->faceNextCcwHalfEdge(h[6]);
		h[8] = m_pMesh->faceNextCcwHalfEdge(h[7]);

		M::CVertex *fv3[3] = {v[2], v[4], pV};
		f[3] = m_pMesh->createFace(fv3, ++m_face_id);
		// new halfedges h[9], h[10], h[11]
		h[9] = m_pMesh->faceHalfedge(f[3]);
		h[10] = m_pMesh->faceNextCcwHalfEdge(h[9]);
		h[11] = m_pMesh->faceNextCcwHalfEdge(h[10]);

		// attach newly created face halfedges to existing edges where necessary
		_attach_halfedge_to_edge(h[0], h[3], eg[0]);
		_attach_halfedge_to_edge(h[1], s[1], eg[1]);
		_attach_halfedge_to_edge(h[5], s[5], eg[5]);

		_attach_halfedge_to_edge(h[7], s[2], eg[2]);
		_attach_halfedge_to_edge(h[10], s[4], eg[4]);

		h[0]->vertex() = v[0];
		h[1]->vertex() = v[1];
		h[2]->vertex() = pV;
		h[3]->vertex() = pV;
		h[4]->vertex() = v[4];
		h[5]->vertex() = v[5];
		h[6]->vertex() = v[1];
		h[7]->vertex() = v[2];
		h[8]->vertex() = pV;
		h[9]->vertex() = v[2];
		h[10]->vertex() = v[4];
		h[11]->vertex() = pV;

		v[0]->halfedge() = h[0];
		v[1]->halfedge() = h[1];
		v[2]->halfedge() = h[7];
		v[4]->halfedge() = h[4];
		pV->halfedge() = h[3];

		for (int k = 0; k < 4; k++)
		{
			M::CHalfEdge *pH = m_pMesh->faceHalfedge(f[k]);
			for (int i = 0; i < 3; i++)
			{
				if (pH->he_sym() != NULL)
					assert(pH->vertex() == pH->he_sym()->he_prev()->vertex());
				pH = m_pMesh->faceNextCcwHalfEdge(pH);
			}
		}

		/*if (pEdge->fixed())
		{
			e[1]->fixed() = true;
		}
		e[1]->sharp() = true;*/

		return pV;
	}

	template <typename M>
	void CTriTopOper<M>::swapEdge(typename M::CEdge *pEdge)
	{
		M::CHalfEdge *he_left = static_cast<M::CHalfEdge *>(pEdge->halfedge(0));
		M::CHalfEdge *he_right = static_cast<M::CHalfEdge *>(pEdge->halfedge(1));

		if (he_right == NULL)
		{
			std::cerr << "Ilegal: Boundary Edge Swap" << std::endl;
			return;
		}

		M::CVertex *v1 = static_cast<M::CVertex *>(m_pMesh->edgeVertex1(pEdge));
		M::CVertex *v2 = static_cast<M::CVertex *>(m_pMesh->edgeVertex2(pEdge));

		if (v1 == v2)
		{
			std::cerr << "Warning: Ilegal: Loop Edge Swap" << std::endl;
			// return;
		}

		M::CHalfEdge *ph[6];

		ph[0] = he_left;
		ph[1] = static_cast<M::CHalfEdge *>(m_pMesh->faceNextCcwHalfEdge(ph[0]));
		ph[2] = static_cast<M::CHalfEdge *>(m_pMesh->faceNextCcwHalfEdge(ph[1]));

		ph[3] = he_right;
		ph[4] = static_cast<M::CHalfEdge *>(m_pMesh->faceNextCcwHalfEdge(ph[3]));
		ph[5] = static_cast<M::CHalfEdge *>(m_pMesh->faceNextCcwHalfEdge(ph[4]));

		M::CVertex *pv[4];
		pv[0] = v1;																												 // 原边端点1
		pv[1] = v2;																												 // 原边端点2
		pv[2] = static_cast<M::CVertex *>(m_pMesh->halfedgeTarget(ph[1])); // 左三角形第三个顶点
		pv[3] = static_cast<M::CVertex *>(m_pMesh->halfedgeTarget(ph[4])); // 右三角形第三个顶点

		// 检查是否会产生重边
		if (pv[2] == pv[3])
		{
			std::cerr << "Error: Edge swap would create degenerate triangle" << std::endl;
			return;
		}

		// 交换后的边连接 v3 和 v4

		// 修改半边的目标顶点（target vertex）
		// 左三角形：v3 -> v4 -> v1
		ph[0]->target() = pv[3];
		ph[0]->target() = pv[3]; // v3 -> v4
		ph[1]->target() = pv[0]; // v4 -> v1
		ph[2]->target() = pv[2]; // v1 -> v3

		ph[3]->target() = pv[2]; // v4 -> v3
		ph[4]->target() = pv[1]; // v3 -> v2
		ph[5]->target() = pv[3]; // v2 -> v4

		// 更新顶点的半边指针，确保每个顶点都指向一个以它为source的有效半边
		// 需要检查原来的半边指针是否仍然有效
		if (pv[0]->halfedge() == ph[0] || pv[0]->halfedge() == ph[3])
		{
			pv[0]->halfedge() = ph[1]; // v1的出边
		}
		if (pv[1]->halfedge() == ph[0] || pv[1]->halfedge() == ph[3])
		{
			pv[1]->halfedge() = ph[4]; // v2的出边
		}
		if (pv[2]->halfedge() == NULL || pv[2]->halfedge()->target() != pv[2])
		{
			pv[2]->halfedge() = ph[0]; // v3的出边（交换后的边）
		}
		if (pv[3]->halfedge() == NULL || pv[3]->halfedge()->target() != pv[3])
		{
			pv[3]->halfedge() = ph[3]; // v4的出边（交换后的边）
		}
	}

	template <typename M>
	void CTriTopOper<M>::generalEdgeSwap(typename M::CEdge *edge, typename M::CVertex *v1, typename M::CVertex *v2)
	{
		// 通用多边形边交换方法
		// edge: 要交换的边
		// v1, v2: 交换后边的两个端点

		// 获取边的两个半边
		M::CHalfEdge *he_left = static_cast<M::CHalfEdge *>(edge->halfedge(0));
		M::CHalfEdge *he_right = static_cast<M::CHalfEdge *>(edge->halfedge(1));

		// 边界检查
		if (he_right == NULL)
		{
			std::cerr << "Error: Cannot swap boundary edge" << std::endl;
			return;
		}

		if (v1 == NULL || v2 == NULL)
		{
			std::cerr << "Error: Invalid vertices" << std::endl;
			return;
		}

		if (v1 == v2)
		{
			std::cerr << "Error: Cannot swap edge to same vertex" << std::endl;
			return;
		}

		// 获取左侧面的所有半边
		std::vector<M::CHalfEdge *> left_halfedges;
		M::CHalfEdge *h = he_left;
		M::CFace *left_face = static_cast<M::CFace *>(h->face());
		do
		{
			left_halfedges.push_back(h);
			h = static_cast<M::CHalfEdge *>(m_pMesh->faceNextCcwHalfEdge(h));
		} while (h != he_left);

		// 获取右侧面的所有半边
		std::vector<M::CHalfEdge *> right_halfedges;
		h = he_right;
		M::CFace *right_face = static_cast<M::CFace *>(h->face());
		do
		{
			right_halfedges.push_back(h);
			h = static_cast<M::CHalfEdge *>(m_pMesh->faceNextCcwHalfEdge(h));
		} while (h != he_right);

		// 验证v1和v2是否在相邻的两个多边形中
		bool v1_in_left = false, v1_in_right = false;
		bool v2_in_left = false, v2_in_right = false;

		for (size_t i = 0; i < left_halfedges.size(); i++)
		{
			M::CVertex *v = static_cast<M::CVertex *>(m_pMesh->halfedgeTarget(left_halfedges[i]));
			if (v == v1)
				v1_in_left = true;
			if (v == v2)
				v2_in_left = true;
		}

		for (size_t i = 0; i < right_halfedges.size(); i++)
		{
			M::CVertex *v = static_cast<M::CVertex *>(m_pMesh->halfedgeTarget(right_halfedges[i]));
			if (v == v1)
				v1_in_right = true;
			if (v == v2)
				v2_in_right = true;
		}

		// v1和v2必须分别在两个面中，且不能同时在同一个面
		if (!((v1_in_left && v2_in_right) || (v1_in_right && v2_in_left)))
		{
			std::cerr << "Error: Vertices must be in adjacent polygons" << std::endl;
			return;
		}

		// 找到v1和v2对应的半边位置
		int v1_idx = -1, v2_idx = -1;
		std::vector<M::CHalfEdge *> *v1_face_hes, *v2_face_hes;

		if (v1_in_left)
		{
			v1_face_hes = &left_halfedges;
			for (size_t i = 0; i < left_halfedges.size(); i++)
			{
				if (m_pMesh->halfedgeTarget(left_halfedges[i]) == v1)
				{
					v1_idx = i;
					break;
				}
			}
		}
		else
		{
			v1_face_hes = &right_halfedges;
			for (size_t i = 0; i < right_halfedges.size(); i++)
			{
				if (m_pMesh->halfedgeTarget(right_halfedges[i]) == v1)
				{
					v1_idx = i;
					break;
				}
			}
		}

		if (v2_in_left)
		{
			v2_face_hes = &left_halfedges;
			for (size_t i = 0; i < left_halfedges.size(); i++)
			{
				if (m_pMesh->halfedgeTarget(left_halfedges[i]) == v2)
				{
					v2_idx = i;
					break;
				}
			}
		}
		else
		{
			v2_face_hes = &right_halfedges;
			for (size_t i = 0; i < right_halfedges.size(); i++)
			{
				if (m_pMesh->halfedgeTarget(right_halfedges[i]) == v2)
				{
					v2_idx = i;
					break;
				}
			}
		}

		if (v1_idx == -1 || v2_idx == -1)
		{
			std::cerr << "Error: Cannot find vertices in faces" << std::endl;
			return;
		}

		// 交换边：将边的两个半边重新定向到v1和v2
		// 通过修改半边的target指针实现
		he_left->target() = (v1_in_left ? v2 : v1);
		he_right->target() = (v1_in_left ? v1 : v2);

		// 更新v1和v2的半边指针
		v1->halfedge() = (v1_in_left ? he_right : he_left);
		v2->halfedge() = (v1_in_left ? he_left : he_right);

		// 更新原来的边端点的半边指针（如果它们指向了被交换的边）
		M::CVertex *old_v1 = static_cast<M::CVertex *>(m_pMesh->edgeVertex1(edge));
		M::CVertex *old_v2 = static_cast<M::CVertex *>(m_pMesh->edgeVertex2(edge));

		if (old_v1 != v1 && old_v1 != v2)
		{
			// 找到old_v1的一个有效出边
			for (size_t i = 0; i < left_halfedges.size(); i++)
			{
				M::CVertex *src = static_cast<M::CVertex *>(
						m_pMesh->halfedgeTarget(m_pMesh->halfedgePrev(left_halfedges[i])));
				if (src == old_v1 && left_halfedges[i] != he_left)
				{
					old_v1->halfedge() = left_halfedges[i];
					break;
				}
			}
		}

		if (old_v2 != v1 && old_v2 != v2)
		{
			// 找到old_v2的一个有效出边
			for (size_t i = 0; i < right_halfedges.size(); i++)
			{
				M::CVertex *src = static_cast<M::CVertex *>(
						m_pMesh->halfedgeTarget(m_pMesh->halfedgePrev(right_halfedges[i])));
				if (src == old_v2 && right_halfedges[i] != he_right)
				{
					old_v2->halfedge() = right_halfedges[i];
					break;
				}
			}
		}
	}

	template <typename M>
	typename M::CVertex *CTriTopOper<M>::addVertexFaceBoundary(typename M::CHalfEdge *phe)
	{
		M::CVertex *pV = m_pMesh->createVertex(++m_vertex_id);

		M::CFace *f = new M::CFace();
		f->id() = ++m_face_id;
		m_pMesh->faces().push_back(f);
		m_pMesh->map_face().insert(pair<int, M::CFace *>(f->id(), f));

		M::CHalfEdge *h[3];
		M::CVertex *v[3];
		M::CEdge *e[3];

		e[0] = m_pMesh->halfedgeEdge(phe);
		e[1] = new M::CEdge();
		e[2] = new M::CEdge();

		v[0] = m_pMesh->halfedgeSource(phe);
		v[1] = pV;
		v[2] = m_pMesh->halfedgeTarget(phe);

		h[0] = new M::CHalfEdge();
		h[1] = new M::CHalfEdge();
		h[2] = new M::CHalfEdge();

		/* link halfedges in one triangle */
		for (int i = 0; i < 3; i++)
		{
			h[i]->he_next() = h[(i + 1) % 3];
			h[(i + 1) % 3]->he_prev() = h[i];
		}
		/* link between halfedge and Face */
		for (int i = 0; i < 3; i++)
		{
			h[i]->face() = f;
		}
		f->halfedge() = h[0];
		/* link halfedge with edge */
		_attach_halfedge_to_edge(h[0], phe, e[0]);
		_attach_halfedge_to_edge(h[1], NULL, e[1]);
		_attach_halfedge_to_edge(h[2], NULL, e[2]);
		/* link vertex and halfedge*/
		h[0]->vertex() = v[0];
		h[1]->vertex() = v[1];
		h[2]->vertex() = v[2];
		v[1]->halfedge() = h[1];
		v[2]->halfedge() = h[2];
		/* tag boundary */
		pV->boundary() = true;
		/* return */
		return pV;
	}

	template <typename M>
	typename M::CHalfEdge *CTriTopOper<M>::addFace2Boundary(typename M::CHalfEdge *phe)
	{
		M::CFace *f = new M::CFace();
		f->id() = ++m_face_id;
		m_pMesh->faces().push_back(f);
		m_pMesh->map_face().insert(pair<int, M::CFace *>(f->id(), f));

		M::CVertex *v[3];
		M::CHalfEdge *hs[2];
		M::CHalfEdge *h[3];
		M::CEdge *e[3];

		v[0] = m_pMesh->halfedgeSource(phe);
		v[2] = m_pMesh->halfedgeTarget(phe);
		hs[0] = phe;
		hs[1] = m_pMesh->vertexMostClwOutHalfEdge(v[2]);
		v[1] = m_pMesh->halfedgeTarget(hs[1]);
		h[0] = new M::CHalfEdge();
		h[1] = new M::CHalfEdge();
		h[2] = new M::CHalfEdge();
		e[0] = m_pMesh->halfedgeEdge(phe);
		e[1] = new M::CEdge();
		e[2] = m_pMesh->halfedgeEdge(hs[1]);

		/* link halfedges in one triangle */
		for (int i = 0; i < 3; i++)
		{
			h[i]->he_next() = h[(i + 1) % 3];
			h[(i + 1) % 3]->he_prev() = h[i];
		}
		/* link between halfedge and Face */
		for (int i = 0; i < 3; i++)
		{
			h[i]->face() = f;
		}
		f->halfedge() = h[0];
		/* link halfedge with edge */
		_attach_halfedge_to_edge(h[0], hs[0], e[0]);
		_attach_halfedge_to_edge(h[1], NULL, e[1]);
		_attach_halfedge_to_edge(h[2], hs[1], e[2]);
		/* link vertex and halfedge*/
		h[0]->vertex() = v[0];
		h[1]->vertex() = v[1];
		h[2]->vertex() = v[2];
		v[1]->halfedge() = h[1];
		/* untag boudnary vertex*/
		v[2]->boundary() = false;
		/* return */
		return h[1];
	}

}

#endif
