#include <graphics.h>
#include <conio.h>
#include "GameModel.h"
#include "tools.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

ChessData game; // ��Ϸָ��
int clickPosRow, clickPosCol; // �洢�����λ��

IMAGE chessBlackImg;
IMAGE chessWhiteImg;

#define INIT_SCORE  1000
int score; // ��ǰ����

void initScore() {
	// ��ʾ��������������
	settextcolor(WHITE);
	settextstyle(50, 0, "΢���ź�");

	FILE *fp = fopen("score.data", "rb");
	if (fp == NULL) {
		score = INIT_SCORE;
	} else {
		fread(&score, sizeof(score), 1, fp);
	}
	if (fp)fclose(fp);
}

void init() {
	initgraph(897, 895);
	loadimage(0, "res/����2.jpg");

	mciSendString("play res/start.wav", 0, 0, 0);

	loadimage(&chessBlackImg, "res/black.png", BLOCK_SIZE, BLOCK_SIZE, true);
	loadimage(&chessWhiteImg, "res/white.png", BLOCK_SIZE, BLOCK_SIZE, true);

	// ��ʼ����Ϸģ��
	initChessData(&game);

	initScore();
}

void chessDown(int row, int col, chess_kind_t kind) {
	mciSendString("play res/down8.WAV", 0, 0, 0);

	int x = margin_x + col * BLOCK_SIZE - 0.5 * BLOCK_SIZE;
	int y = margin_y + row * BLOCK_SIZE - 0.5 * BLOCK_SIZE;

	if (kind == CHESS_WHITE) {
		drawPNG(&chessWhiteImg, x, y);
	}
	else {
		drawPNG(&chessBlackImg, x, y);
	}
}

void manGo() {
	chessDown(clickPosRow, clickPosCol, CHESS_BLACK);
	
	updateGameMap(&game, clickPosRow, clickPosCol);
}

bool checkOver() {
	if (checkWin(&game, clickPosRow, clickPosCol)) {
		Sleep(1500);
		if (game.playerFlag == false) {  //����Ӯ�����Ӯ��,��ʱ����Ѿ���ת���ֵ���������
			mciSendString("play res/����.mp3", 0, 0, 0);
			loadimage(0, "res/ʤ��.jpg");
			score += 100;
		} else {
			mciSendString("play res/ʧ��.mp3", 0, 0, 0);
			loadimage(0, "res/ʧ��.jpg");
			score -= 100;
		}

		// ��ʾ����
		char scoreText[64]; 
		sprintf(scoreText, "��ǰ���� ��%d", score);
		outtextxy(310, 800, scoreText);

		// ��¼����
		FILE* fp = fopen("score.data", "wb");
		fwrite(&score, sizeof(score), 1, fp);
		fclose(fp);
		
		getch();
		return true;
	}
	return false;
}

bool clickBoard(MOUSEMSG msg) {
	int x = msg.x;
	int y = msg.y;
	
	int col = (x - MARGIN) / BLOCK_SIZE;
	int row = (y - MARGIN) / BLOCK_SIZE;

	int leftTopPosX = MARGIN + BLOCK_SIZE * col;
	int leftTopPosY = MARGIN + BLOCK_SIZE * row;

	int len;
	int selectPos = false;
	
	do {
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < POS_OFFSET) {
			clickPosRow = row;
			clickPosCol = col;
			if (game.chessMap[clickPosRow][clickPosCol] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������Ͻǵľ���
		len = sqrt((x - leftTopPosX - BLOCK_SIZE) * (x - leftTopPosX - BLOCK_SIZE) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < POS_OFFSET) {
			clickPosRow = row;
			clickPosCol = col + 1;
			if (game.chessMap[clickPosRow][clickPosCol] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������½ǵľ���
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - BLOCK_SIZE) * (y - leftTopPosY - BLOCK_SIZE));
		if (len < POS_OFFSET) {
			clickPosRow = row + 1;
			clickPosCol = col;
			if (game.chessMap[clickPosRow][clickPosCol] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������½ǵľ���
		len = sqrt((x - leftTopPosX - BLOCK_SIZE) * (x - leftTopPosX - BLOCK_SIZE) + (y - leftTopPosY - BLOCK_SIZE) * (y - leftTopPosY - BLOCK_SIZE));
		if (len < POS_OFFSET) {
			clickPosRow = row + 1;
			clickPosCol = col + 1;

			if (game.chessMap[clickPosRow][clickPosCol] == 0) {
				selectPos = true;
			}
			break;
		}
	} while (0);

	return selectPos;
}

void AI_GO() {
	point_t point = actionByAI(&game);
	clickPosRow = point.row;
	clickPosCol = point.col;

	Sleep(1000); //AI�����̫�죬�˴��Լ�װ˼��
	chessDown(clickPosRow, clickPosCol, CHESS_WHITE);
	updateGameMap(&game, clickPosRow, clickPosCol);
}


int main(void) {
	init();

	while (1) {
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN  && clickBoard(msg)) {
			manGo();
			if (checkOver()) {
				init();
				continue;
			}

			AI_GO();
			if (checkOver()) {
				init();
				continue;
			}
		}
	}

	closegraph();
	return 0;
}