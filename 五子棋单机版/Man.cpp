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
		//��ȡ�������Ϣ
		msg = GetMouseMsg();
		//ͨ������chess���󣬵��ã��ж������Ƿ���Ч���Լ����ӹ���
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos)) {
			break;
		}
	}
	std::cout << "���壺(" << pos.col << "," << pos.row << ")" << std::endl;
	//����������ʾ���壨���ӣ�
	chess->chessDown(&pos, CHESS_BLACK);
	

}
