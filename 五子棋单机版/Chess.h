#pragma once
#include<graphics.h> //easyx图形库的头文件
#include<vector>
#include<iostream>
using namespace std;

//落子位置的结构体
struct ChessPos {
	int row;//行
	int col;//列

	ChessPos(int r = 0, int c = 0) :row(r), col(c) {}
};
typedef enum
{
	CHESS_WHITE = -1,//白棋
	CHESS_BLACK = 1//黑棋
} chess_kind_t;

class Chess
{
public:
	Chess(int gradeSize, int marginX, int marginY, float chesssSize);

	void init();

	//如果是有效点击，把有效点击位置返回给参数pos
	bool clickBoard(int x, int y, ChessPos* pos); //棋盘的点击位置

	//在期盼指定位置pos，落子（kind)
	void chessDown(ChessPos* pos, chess_kind_t kind);//下棋位置

	// 获取棋盘大小（13，15，19）
	int getGradeSize(); 

	//获取指定位置是黑棋还是白棋，还是空白
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	//检查游戏是否结束
	bool checkOver();

	//更新棋盘数据
	void updateChessMap(ChessPos *pos);

	//如果胜负已分，返回true，否则false
	bool checkWin();

//使用图形库（下载安装easyx），显示棋盘和棋子	
private:
	IMAGE chessBlackImg;	//黑棋
	IMAGE chessWhiteImg;	//白棋
	IMAGE lastChessDownPosImg;
	ChessPos lastlastDownPos;

	int gradeSize;	//棋盘大小
	int margin_x;	//棋盘的左侧边界
	int margin_y;	//棋盘的顶部边界
	float chessSize;	//棋盘方格大小

	//存储当前棋局的棋子分步数据：0->空白	-1->黑棋	1->白棋
	vector<vector<int>> chessMap;

	//表示现在该谁下棋
	bool playerFlag;	//true 是该黑棋下，false是该白棋下

	//最后的落子点位置
	ChessPos lastPos;


};


