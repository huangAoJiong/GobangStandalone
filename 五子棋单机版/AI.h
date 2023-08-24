#pragma once
#include"Chess.h"
#include<vector>
class AI
{
public:
	void init(Chess *chess);
	void go();//��ʼ��Ϸ
private:
	Chess *chess;
	vector<vector<int>> scoreMap;

//private:
	void calculateScore();
	ChessPos think();
};

