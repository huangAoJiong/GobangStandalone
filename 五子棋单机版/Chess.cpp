#include "Chess.h"
#include<graphics.h>
#include<conio.h>
#pragma comment(lib,"winmm.lib")
#include <mmsystem.h> // 包含 Windows Multimedia API 头文件
#include<windows.h>
void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}




Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
    :gradeSize(gradeSize),margin_x(marginX),margin_y(marginY),chessSize(chessSize)
{
    //默认黑棋先走
    playerFlag = CHESS_BLACK;
	lastlastDownPos.col = -1;
	lastlastDownPos.row = -1;
    //初始化棋盘（vector容器）
    for (int i = 0; i < gradeSize; i++) {
        vector<int>row;
        for (int j = 0; j < gradeSize; j++) {
            row.push_back(0);
        }
        chessMap.push_back(row);
    }
}

void Chess::init()
{
    //创建游戏窗口
    initgraph(1024,1024, EX_SHOWCONSOLE);

    //显示棋盘
    loadimage(0, L"res/棋盘.jpg");

	mciSendString(L"play res/start.WAV",0,0,0);

    //加载黑棋白棋图片
    loadimage(&chessBlackImg, L"res/black.png",chessSize,chessSize,true);
    loadimage(&chessWhiteImg, L"res/white.png", chessSize, chessSize, true);
	loadimage(&lastChessDownPosImg,L"res/lastDown.png", chessSize, chessSize, true);
    //棋盘清零
    for (int i = 0; i < gradeSize; i++) {
        for (int j = 0; j < gradeSize; j++) {
            chessMap[i][j] = 0;
        }
    }

}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	int col = (x - margin_x) / chessSize;
	int row = (y - margin_y) / chessSize;

	int leftTopPosX = margin_x + chessSize * col;
	int leftTopPosY = margin_y + chessSize * row;
	int offset = chessSize * 0.4; // 20 鼠标点击的模糊距离上限

	int len;
	int selectPos = false;

	do {
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			pos->row = row;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// 距离右上角的距离
		len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			pos->row = row;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// 距离左下角的距离
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// 距离右下角的距离
		len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col + 1;

			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}
	} while (0);

	return selectPos;
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
	int x = margin_x + chessSize * pos->col - 0.5 * chessSize +7;
	int y = margin_y + chessSize * pos->row - 0.5 * chessSize+6;
	if (kind == CHESS_WHITE) {
		putimagePNG(x, y, &chessWhiteImg);
	}
	else {
		putimagePNG(x, y, &chessBlackImg);
	}
	mciSendString(L"play res/down8.wav", 0, 0, 0);
	updateChessMap(pos);

	putimagePNG(x, y, &lastChessDownPosImg);
	lastlastDownPos = *pos;
	//if (lastlastDownPos.col != -1) {
	//	if (kind == CHESS_WHITE) {
	//		putimagePNG(lastlastDownPos.col, lastlastDownPos.row, &chessBlackImg);
	//	}
	//	else {
	//		putimagePNG(lastlastDownPos.col, lastlastDownPos.row, &chessWhiteImg);
	//	}
	//}


}

int Chess::getGradeSize()
{
    return gradeSize;
}

int Chess::getChessData(ChessPos* pos)
{
    return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
    return chessMap[row][col];
}

bool Chess::checkOver()
{
	if (checkWin()) {
		if (playerFlag == false) {//现在是假，就是该白棋走了，那么就是黑棋赢了
			loadimage(0, L"res/victory.jpg");
			mciSendString(L"play res/不错胜利.mp3", 0, 0, 0);
		}
		else {//现在是true，那么就是白棋赢了
			loadimage(0, L"res/loser.jpg");
			mciSendString(L"play res/失败.mp3", 0, 0, 0);
		}
		std::cout << (playerFlag == false ? "玩家胜利" : "玩家失败，AI胜利") << std::endl;
		_getch();	//暂停，按任意键继续
		return true;
	}
    return false;
}

void Chess::updateChessMap(ChessPos* pos)
{
	chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
	playerFlag = !playerFlag;
	lastPos = *pos;
}

bool Chess::checkWin()
{
	int row = lastPos.row;
	int col = lastPos.col;


	//落子水平方向
	for (int i = 0; i < 5; i++) {
		//对于第i次，五子连珠的最左侧位置：
		if (col -i>=0&&
			col-i+4<gradeSize&&
			chessMap[row][col - i] == chessMap[row][col - i + 1] && 
			chessMap[row][col - i] == chessMap[row][col - i + 2] && 
			chessMap[row][col - i] == chessMap[row][col - i + 3] && 
			chessMap[row][col - i] == chessMap[row][col - i + 4]) {
			//已经有了五子连珠
			return true;
		}
	}

	//落子垂直方向
	for (int i = 0; i < 5; i++) {
		//对于第i次，五子连珠的最左侧位置：
		if (row - i >= 0 &&
			row - i + 4 < gradeSize &&
			chessMap[row-i][col] == chessMap[row - i + 1][col] &&
			chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i +3][col] &&
			chessMap[row - i][col] == chessMap[row - i + 4][col]) {
			//已经有了五子连珠
			return true;
		}
	}

	//落子左下右上'/'方向
	for (int i = 0; i < 5; i++) {
		//对于第i次，五子连珠的最左侧位置：
		if (row + i-4 >= 0 &&
			row +i < gradeSize &&
			col-i>=0 && col-i+4<gradeSize&&
			chessMap[row + i][col-i] == chessMap[row + i -1][col-i+1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col-i+2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col-i+3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col-i+4]) {
			//已经有了五子连珠
			return true;
		}
	}

	//落子右下左上‘\’方向
	for (int i = 0; i < 5; i++) {
		//对于第i次，五子连珠的最左侧位置：
		if (row - i  >= 0 && row - i +4< gradeSize &&
			col - i >= 0 && col - i + 4 < gradeSize &&
			chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] &&
			chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] &&
			chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4]) {
			//已经有了五子连珠
			return true;
		}
	}



	return false;
}


