#ifndef _VIEWER_KEY_INTER_H_
#define _VIEWER_KEY_INTER_H_

#include<vector>
using namespace std;

namespace Viewer
{
	
	class CKeyInter
	{
	public:
		CKeyInter(unsigned char _key) 
		{
			m_key = _key;
		};
		~CKeyInter() {};
	public:
		virtual void keyInter(int _x, int _y) = 0;
		unsigned char& key() { return m_key; };
	protected:
		unsigned char m_key;
	};

	class CKeyBdInter
	{
	public:
		CKeyBdInter() {};
		~CKeyBdInter() {};
	private:
		vector<CKeyInter*> keyInterVector;
	public:
		void addKeyInter(CKeyInter* pKeyInter)
		{
			keyInterVector.push_back(pKeyInter);
		}
		void keyBdInter(unsigned char _key, int _x, int _y)
		{
			int size = keyInterVector.size();
			for (int i = 0; i < size; i++)
			{
				if (_key != keyInterVector[i]->key())
					continue;
				keyInterVector[i]->keyInter(_x, _y);
			}
		}
		void clearKeyInter()
		{
			keyInterVector.clear();
		}
	};

}






#endif
