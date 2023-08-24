#pragma once
#include"Chess.h"
#include<vector>
class AI
{
public:
	void init(Chess *chess);
	void go();//©╙й╪сно╥
private:
	Chess *chess;
	vector<vector<int>> scoreMap;

//private:
	void calculateScore();
	ChessPos think();
};

