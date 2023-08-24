#include "Man.h"
#include<iostream>
void Man::init(Chess* chess)
{
	this->chess = chess;

}

void Man::go()
{
	MOUSEMSG msg;
	ChessPos pos;

	while (1) {
		//获取鼠标点击消息
		msg = GetMouseMsg();
		//通过棋盘chess对象，调用，判断落子是否有效，以及落子功能
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos)) {
			break;
		}
	}
	std::cout << "黑棋：(" << pos.col << "," << pos.row << ")" << std::endl;
	//在棋盘中显示黑棋（落子）
	chess->chessDown(&pos, CHESS_BLACK);
	

}
