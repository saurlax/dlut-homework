#ifndef _ViewerParas_H_
#define _ViewerParas_H_

namespace Viewer
{
	class CViewerParas
	{
	public:
		CViewerParas()
		{
			mShow = true;
			mShowVertex = false;
			mShowEdge = false;
			mShowFace = true;
			useFaceNormal = false;

			computeNormal = true;
		};
		~CViewerParas() {};
	public:/*基本显示控制*/
		bool mShow;
		bool mShowVertex;
		bool mShowEdge;
		bool mShowFace;
		bool useFaceNormal;
		/* 预处理控制及相关属性
		   相关操作只执行一次
		   操作后立即更改控制参数*/
		bool computeNormal;
	};
}




#endif
