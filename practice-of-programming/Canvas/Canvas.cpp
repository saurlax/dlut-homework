#include <graphics.h>
#include <string>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926

using namespace std;

int main()
{
	bool running = true;
	bool updated = true;

	bool square = false;
	bool circle = false;
	int width = 10, x, y;

	ExMessage msg;

	initgraph(800, 600);

	cleardevice();

	outtextxy(20, 20, L"Canvas");
	outtextxy(20, 60, L"LMOUSE: SQUARE    RMOUSE: CIRCLE");
	outtextxy(20, 80, L"R: RED    G: GREEN    B: BLUE    W: WHITE");
	outtextxy(20, 100, L"C: CLEAR    ESC: ESCAPE    CTRL: THICKEN");

	while (running) {
		while (peekmessage(&msg)) {
			if (msg.message == WM_CLOSE) running = false;
			else if (msg.message == WM_KEYDOWN) {
				switch (msg.vkcode) {
				case VK_ESCAPE:
					running = false;
					break;
				case VK_CONTROL:
					width = 20;
					break;
				case 'c':
				case 'C':
					cleardevice();
					break;
				case 'r':
				case 'R':
					setfillcolor(RED);
					break;
				case 'g':
				case 'G':
					setfillcolor(GREEN);
					break;
				case 'b':
				case 'B':
					setfillcolor(BLUE);
					break;
				case 'w':
				case 'W':
					setfillcolor(WHITE);
					break;
				}
			}
			else if (msg.message == WM_KEYUP && msg.vkcode == VK_CONTROL) {
				width = 10;
			}
			else if (msg.message == WM_LBUTTONDOWN) {
				square = true;
				updated = true;
			}
			else if (msg.message == WM_RBUTTONDOWN) {
				circle = true;
				updated = true;
			}
			else if (msg.message == WM_LBUTTONUP) {
				square = false;
				updated = true;
			}
			else if (msg.message == WM_RBUTTONUP) {
				circle = false;
				updated = true;
			}
			else if (msg.message == WM_MOUSEMOVE) {
				x = msg.x, y = msg.y;
				updated = true;
			}
		}

		if (updated) {
			updated = false;
			BeginBatchDraw();
			if (square) {
				solidrectangle(x - width / 2, y - width / 2, x + width / 2, y + width / 2);
			}
			if (circle) {
				solidcircle(x, y, width);
			}
			EndBatchDraw();
		}
	}

	closegraph();
	return 0;
}