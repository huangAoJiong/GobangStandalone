#pragma once
#include<graphics.h> //easyxͼ�ο��ͷ�ļ�
#include<vector>
#include<iostream>
using namespace std;

//����λ�õĽṹ��
struct ChessPos {
	int row;//��
	int col;//��

	ChessPos(int r = 0, int c = 0) :row(r), col(c) {}
};
typedef enum
{
	CHESS_WHITE = -1,//����
	CHESS_BLACK = 1//����
} chess_kind_t;

class Chess
{
public:
	Chess(int gradeSize, int marginX, int marginY, float chesssSize);

	void init();

	//�������Ч���������Ч���λ�÷��ظ�����pos
	bool clickBoard(int x, int y, ChessPos* pos); //���̵ĵ��λ��

	//������ָ��λ��pos�����ӣ�kind)
	void chessDown(ChessPos* pos, chess_kind_t kind);//����λ��

	// ��ȡ���̴�С��13��15��19��
	int getGradeSize(); 

	//��ȡָ��λ���Ǻ��廹�ǰ��壬���ǿհ�
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	//�����Ϸ�Ƿ����
	bool checkOver();

	//������������
	void updateChessMap(ChessPos *pos);

	//���ʤ���ѷ֣�����true������false
	bool checkWin();

//ʹ��ͼ�ο⣨���ذ�װeasyx������ʾ���̺�����	
private:
	IMAGE chessBlackImg;	//����
	IMAGE chessWhiteImg;	//����
	IMAGE lastChessDownPosImg;
	ChessPos lastlastDownPos;

	int gradeSize;	//���̴�С
	int margin_x;	//���̵����߽�
	int margin_y;	//���̵Ķ����߽�
	float chessSize;	//���̷����С

	//�洢��ǰ��ֵ����ӷֲ����ݣ�0->�հ�	-1->����	1->����
	vector<vector<int>> chessMap;

	//��ʾ���ڸ�˭����
	bool playerFlag;	//true �Ǹú����£�false�Ǹð�����

	//�������ӵ�λ��
	ChessPos lastPos;


};


