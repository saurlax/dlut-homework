#ifndef _TET_TEST_H_
#define _TET_TEST_H_

using namespace std;

namespace TMeshLib
{
	template<typename M>
	class CTetTest
	{
	public:
		CTetTest(M* pMesh);
		~CTetTest();		
		
		void test();
	protected:
		M* m_pMesh;		
	};

	template<typename M>
	CTetTest<M>::CTetTest(M* pMesh)
	{
		m_pMesh = pMesh;
		for (M::MeshVertexIterator mv(m_pMesh); !mv.end(); mv++)
		{
			M::CVertex* pVertex = *mv;
			pVertex->_from_string();
		}		
	}

	template<typename M>
	CTetTest<M>::~CTetTest()
	{
	
	}	

	template<typename M>
	void CTetTest<M>::test()
	{
		for (M::MeshVertexIterator mv(m_pMesh); !mv.end(); mv++)
		{
			M::CVertex* pVertex = mv.value();
			CPoint p = pVertex->position();
			pVertex->position()[0]*= 2;
			pVertex->position()[1]*= 2;
		}
	}
};
#endif