#ifndef _TET_TEST_MESH_H_
#define _TET_TEST_MESH_H_

#include"../TMeshLib/Mesh/basetmesh.h"
#include"../TMeshLib/Mesh/tvertex.h"
#include"../TMeshLib/Mesh/vertex.h"
#include"../TMeshLib/Mesh/halfedge.h"
#include"../TMeshLib/Mesh/tedge.h"
#include"../TMeshLib/Mesh/edge.h"
#include"../TMeshLib/Mesh/halfface.h"
#include"../TMeshLib/Mesh/face.h"
#include"../TMeshLib/Mesh/tet.h"
#include"../TMeshLib/Mesh/titerators.h"

#include"../TMeshLib/Parser/parser.h"
#include"../TMeshLib/Geometry/Point.h"
#include"../TMeshLib/Geometry/Point2.h"

using namespace std;

namespace TMeshLib
{

	class CTetTestVertex : public CVertex
	{
	public:
		CTetTestVertex(){
			m_u = 0.5;
		};
		~CTetTestVertex(){};
	public:		
		double & u(){ return m_u; };
		void _to_string();
		void _from_string();		
	protected:		
		double m_u;		
	};


	inline void CTetTestVertex::_from_string()
	{
		CParser parser(m_string);
		for (std::list<CToken*>::iterator iter = parser.tokens().begin(); iter != parser.tokens().end(); ++iter)
		{
			CToken * token = *iter;
			if (token->m_key == "u")
			{
				double u;
				sscanf_s(token->m_value.c_str(), "(%lf)", &u);
				m_u = u;
			}
		}
	}


	inline void CTetTestVertex::_to_string()
	{
		if (0)
		{
			CParser parser(m_string);
			parser._removeToken("u");
			parser._toString(m_string);
			std::stringstream iss;
			iss << "u=(" << m_u << ")";
			if (m_string.length() > 0)
			{
				m_string += " ";
			}
			m_string += iss.str();
		}			
	}
	

	class CTetTestTVertex : public CTVertex
	{
	public:
		CTetTestTVertex(){};
		~CTetTestTVertex(){};		
	protected:
		
	};


	class CTetTestHalfEdge : public CHalfEdge
	{
	public:
		CTetTestHalfEdge(){};
		~CTetTestHalfEdge(){};
	protected:

	};


	class CTetTestTEdge : public CTEdge
	{
	public:
		CTetTestTEdge(){};
		~CTetTestTEdge(){};
	public:
		double & angle(){ return m_angle; };
	protected:
		double m_angle;
	};


	class CTetTestEdge : public CEdge
	{
	public:
		CTetTestEdge(){};
		~CTetTestEdge(){};
	public:
		double & weight(){ return m_weight; };
		double & length(){ return m_length; };
	protected:
		double m_weight;
		double m_length;
	};


	class CTetTestHalfFace : public CHalfFace
	{
	public:
		CTetTestHalfFace(){
		};
		~CTetTestHalfFace(){};
	public:
		CPoint & normal(){ return m_normal; };
	protected:
		CPoint m_normal;
	};


	class CTetTestFace : public CFace
	{
	public:
		CTetTestFace(){};
		~CTetTestFace(){};
	public:
		double & area(){ return m_area; };
	protected:
		double m_area;
	};


	class CTetTestTet : public CTet
	{
	public:
		CTetTestTet(){};
		~CTetTestTet(){};
	protected:
	};

	template<typename TV,typename V,typename HE,typename TE,typename E,typename HF,typename F,typename T>
	class CTetTestTMesh : public CTMesh<TV, V, HE, TE, E, HF, F, T>
	{
	public:
		typedef TV CTVertex;
		typedef V CVertex;
		typedef HE CHalfEdge;
		typedef TE CTEdge;
		typedef E CEdge;
		typedef HF CHalfFace;
		typedef F CFace;
		typedef T CTet;
	public:
		typedef TMeshEdgeIterator<TV, V, HE, TE, E, HF, F, T> MeshEdgeIterator;
		typedef TMeshTetIterator<TV, V, HE, TE, E, HF, F, T> MeshTetIterator;
		typedef TMeshVertexIterator<TV, V, HE, TE, E, HF, F, T> MeshVertexIterator;
		typedef TMeshFaceIterator<TV, V, HE, TE, E, HF, F, T> MeshFaceIterator;
		typedef EdgeTEdgeIterator<TV, V, HE, TE, E, HF, F, T> EdgeTEdgeIterator;
		typedef TVertexVertexIterator<TV, V, HE, TE, E, HF, F, T> VertexVertexIterator;
		typedef TVertexEdgeIterator<TV, V, HE, TE, E, HF, F, T> VertexEdgeIterator;
		typedef FaceVertexIterator<TV, V, HE, TE, E, HF, F, T> FaceVertexIterator;
		typedef VertexTVertexIterator<TV, V, HE, TE, E, HF, F, T> VertexTVertexIterator;

	public:
	};
	
	
	typedef CTetTestTMesh<CTetTestTVertex, CTetTestVertex, CTetTestHalfEdge, CTetTestTEdge, CTetTestEdge, CTetTestHalfFace, CTetTestFace, CTetTestTet> CTTMesh;
	
	

};
#endif