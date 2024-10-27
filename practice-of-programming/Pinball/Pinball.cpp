#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#define G 9.8
#define PI 3.1415926
#define LOSS 1

using namespace std;

bool running = true;
bool playing = false;

int windowWidth = 800, windowHeight = 600;

int brickWidth = 50, brickHeight = 10;
int brickX = 400 - brickWidth / 2;
int brickY = 550;
int brickdX = 0;

int ballRadius = 10;
int ballX = 400 - ballRadius / 2, ballY = 300 - ballRadius / 2;
double balldX = 5.0 * rand() / RAND_MAX;
double balldY = 5.0 * rand() / RAND_MAX - 5;

int score = 0;

void reset() {
	brickX = 400 - brickWidth / 2;
	brickdX = 0;
	
	ballX = 400 - ballRadius / 2, ballY = 300 - ballRadius / 2;
	balldX = 5.0 * rand() / RAND_MAX;
	balldY = 5.0 * rand() / RAND_MAX - 5;

	score = 0;
}

int main()
{
	srand(time(NULL));
	ULONGLONG tick = GetTickCount();
	ExMessage msg;

	initgraph(windowWidth, windowHeight);
	HWND hWnd = GetHWnd();
	SetWindowText(hWnd, L"Pinball: MOVE TO START, BALL LOST AND YOU LOSE");

	while (running) {
		ULONGLONG now = GetTickCount();

		while (peekmessage(&msg)) {
			if (msg.message == WM_CLOSE) running = false;
			else if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
				bool down = msg.message == WM_KEYDOWN;
				switch (msg.vkcode) {
				case VK_ESCAPE:
					running = false;
					break;
				case 'a':
				case 'A':
				case VK_LEFT:
					if (!playing && down) {
						reset();
						playing = true;
					}
					brickdX = down ? -5 : 0;
					break;
				case 'd':
				case 'D':
				case VK_RIGHT:
					if (!playing && down) {
						reset();
						playing = true;
					}
					brickdX = down ? 5 : 0;
					break;
				}
			}
		}

		if (now - tick < 5) continue;
		double fps = 1000.0 / (now - tick);
		tick = now;

		if (playing) {
			balldY += G / fps;

			ballX += balldX;
			ballY += balldY;

			brickX += brickdX;
			brickX = max(0, brickX);
			brickX = min(windowWidth - brickWidth, brickX);

			if (ballX < ballRadius) {
				balldX = -balldX * LOSS;
				ballX = ballRadius;
			}
			else if (ballX > windowWidth - ballRadius) {
				balldX = -balldX * LOSS;
				ballX = windowWidth - ballRadius;
			}
			if (ballY < ballRadius) {
				balldY = -balldY * LOSS;
				ballY = ballRadius;
			}
			else if (ballY > windowHeight - ballRadius) {
				playing = false;
			}

			if (ballX + ballRadius > brickX && ballX - ballRadius < brickX + brickWidth &&
				ballY + ballRadius > brickY && ballY - ballRadius < brickY + brickHeight) {
				ballY = brickY - ballRadius;
				balldY = -balldY * LOSS;
				balldX += brickdX * 0.1;
				score++;
			}
		}

		double speed = sqrt(balldX * balldX + balldY * balldY);

		BeginBatchDraw();

		cleardevice();
		wchar_t text[50];
		swprintf(text, 50, L"FPS: %.1f    SCORE: %d    SPEED: %.2f", fps, score, speed);
		outtextxy(0,0, text);

		setfillcolor(WHITE);
		solidrectangle(brickX, brickY, brickX + brickWidth, brickY + brickHeight);

		setfillcolor(HSLtoRGB(360.0 * ballX / windowWidth * ballY / windowHeight, 0.5, 0.5));
		solidcircle(ballX, ballY, ballRadius);

		EndBatchDraw();
		
	}

	closegraph();
	return 0;
}