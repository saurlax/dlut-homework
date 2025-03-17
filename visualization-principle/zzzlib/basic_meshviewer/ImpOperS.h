#ifndef _IMP_OPERS_H_
#define _IMP_OPERS_H_

#include<vector>

namespace AlgoFrame {

	class CParas
	{
		CParas() {};
		~CParas() {};
	};

	class COper
	{
	public:
		COper() {};
		~COper() {};
	public:
		virtual void impOper() = 0;
		CParas* paras;
	};

	class CImpOperS
	{
	public:
		CImpOperS() {};
		~CImpOperS() {};
		virtual void prepare() {};
	public:
		void impOperS()
		{
			prepare();
			int size = OperVector.size();
			for (int i = 0; i < size; i++)
			{
				OperVector[i]->impOper();
			}
		}
		void addOper(COper* _Oper)
		{
			OperVector.push_back(_Oper);
		}
		void removeLastOper()
		{
			int size = OperVector.size();
			delete OperVector[size - 1];
			OperVector.erase(OperVector.begin() + size - 1);
		}
	public:
		std::vector<COper*> OperVector;
	};


}

#endif