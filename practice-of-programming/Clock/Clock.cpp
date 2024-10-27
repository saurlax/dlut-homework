#include <graphics.h>
#include <string>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PI 3.1415926

using namespace std;

int main()
{
	bool running = true;
	bool updated = true;
	ExMessage msg;
	SYSTEMTIME time;
	long long tick = GetTickCount();

	initgraph(800, 600);
	setorigin(400, 300);

	while (running)
	{
		if (peekmessage(&msg))
		{
			if (msg.message == WM_CLOSE || msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
				running = false;
		}

		GetLocalTime(&time);

		long long now = GetTickCount();
		if (now - tick > 1000)
		{
			tick = now;
			updated = true;
		}

		if (updated)
		{
			updated = false;
			BeginBatchDraw();

			setcolor(BLACK);
			setbkcolor(WHITE);
			cleardevice();

			setlinestyle(PS_SOLID, 3);
			circle(0, 0, 250);
			setlinestyle(PS_SOLID, 1);

			setfillcolor(0xe6e6e6);
			fillcircle(0, 0, 242);

			setbkcolor(0xe6e6e6);

			int r0 = 180, r1 = 222, r2 = 230;
			int fontWidth = 30, fontHeight = 50;
			settextstyle(fontHeight, fontWidth, L"宋体");
			for (int i = 0; i < 60; i++)
			{
				double t = 2 * PI * i / 60 - PI / 3;
				if (i % 5 == 0)
				{
					wchar_t text[3];
					wsprintf(text, L"%d", i / 5 + 1);
					double offsetX = fontWidth / 2.0, offsetY = fontHeight / 2.0;
					if (lstrlen(text) > 1)
					{
						offsetX *= 2;
					}
					outtextxy(r0 * cos(t) - offsetX, r0 * sin(t) - offsetY, text);
					setlinestyle(PS_SOLID, 2);
				}
				line(r1 * cos(t), r1 * sin(t), r2 * cos(t), r2 * sin(t));
				setlinestyle(PS_SOLID, 1);
			}

			int rH = 140, rM = 210, rS = 180;
			setfillcolor(BLACK);
			solidcircle(0, 0, 10);
			double tS = 2 * PI * time.wSecond / 60 - PI / 2;
			double tM = 2 * PI * time.wMinute / 60 + tS / 60 - PI / 2;
			double tH = 2 * PI * time.wHour / 12 + tM / 12 - PI / 2;

			setlinestyle(PS_SOLID, 7);
			line(0, 0, rH * cos(tH), rH * sin(tH));
			line(0, 0, rM * cos(tM), rM * sin(tM));

			setfillcolor(RED);
			solidcircle(0, 0, 6);
			setcolor(RED);
			setlinestyle(PS_SOLID, 3);
			line(0, 0, rS * cos(tS), rS * sin(tS));

			EndBatchDraw();
		}
	}

	closegraph();
	return 0;
}