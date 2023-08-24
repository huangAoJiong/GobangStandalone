#include <graphics.h>
#include <conio.h>
#include "GameModel.h"
#include "tools.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

ChessData game; // 游戏指针
int clickPosRow, clickPosCol; // 存储点击的位置

IMAGE chessBlackImg;
IMAGE chessWhiteImg;

#define INIT_SCORE  1000
int score; // 当前分数

void initScore() {
	// 显示分数的字体设置
	settextcolor(WHITE);
	settextstyle(50, 0, "微软雅黑");

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
	loadimage(0, "res/棋盘2.jpg");

	mciSendString("play res/start.wav", 0, 0, 0);

	loadimage(&chessBlackImg, "res/black.png", BLOCK_SIZE, BLOCK_SIZE, true);
	loadimage(&chessWhiteImg, "res/white.png", BLOCK_SIZE, BLOCK_SIZE, true);

	// 初始化游戏模型
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
		if (game.playerFlag == false) {  //黑棋赢（玩家赢）,此时标记已经反转，轮到白棋落子
			mciSendString("play res/不错.mp3", 0, 0, 0);
			loadimage(0, "res/胜利.jpg");
			score += 100;
		} else {
			mciSendString("play res/失败.mp3", 0, 0, 0);
			loadimage(0, "res/失败.jpg");
			score -= 100;
		}

		// 显示分数
		char scoreText[64]; 
		sprintf(scoreText, "当前分数 ：%d", score);
		outtextxy(310, 800, scoreText);

		// 记录分数
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

		// 距离右上角的距离
		len = sqrt((x - leftTopPosX - BLOCK_SIZE) * (x - leftTopPosX - BLOCK_SIZE) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < POS_OFFSET) {
			clickPosRow = row;
			clickPosCol = col + 1;
			if (game.chessMap[clickPosRow][clickPosCol] == 0) {
				selectPos = true;
			}
			break;
		}

		// 距离左下角的距离
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - BLOCK_SIZE) * (y - leftTopPosY - BLOCK_SIZE));
		if (len < POS_OFFSET) {
			clickPosRow = row + 1;
			clickPosCol = col;
			if (game.chessMap[clickPosRow][clickPosCol] == 0) {
				selectPos = true;
			}
			break;
		}

		// 距离右下角的距离
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

	Sleep(1000); //AI计算的太快，此处以假装思考
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