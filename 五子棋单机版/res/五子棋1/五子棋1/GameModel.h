#pragma once
// ---- ��������Ϸģ���� ---- //
#include <vector>

typedef enum {
    CHESS_WHITE = -1,
    CHESS_BLACK = 1
} chess_kind_t;

// ���̳ߴ�
const int margin_x = 44;//49;
const int margin_y = 43;// 49;

const int BOARD_GRAD_SIZE = 13;
const int MARGIN = 49; // ���̱�Ե��϶
const int CHESS_RADIUS = 15; // ���Ӱ뾶
const int MARK_SIZE = 20; // ���ӱ�Ǳ߳�
const float BLOCK_SIZE = 67.4; // ���ӵĴ�С
const int POS_OFFSET = BLOCK_SIZE * 0.4; // 20 �������ģ����������
const int AI_THINK_TIME = 700; // AI�����˼��ʱ��

struct  ChessData {
    // �洢��ǰ��Ϸ���̺����ӵ����,�հ�Ϊ0������1������-1
    int chessMap[BOARD_GRAD_SIZE][BOARD_GRAD_SIZE];

    // �洢������λ�������������ΪAI��������
    int scoreMap[BOARD_GRAD_SIZE][BOARD_GRAD_SIZE];

    // ��ʾ���巽, true:���巽  false: AI ���巽��AI����
    bool playerFlag;
};

typedef struct point {
    int row;
    int col;
} point_t;

void initChessData(ChessData*); // ��ʼ��Ϸ

void updateGameMap(ChessData* data, int row, int col);

bool checkWin(ChessData* game, int row, int col);

void calculateScore(ChessData* dat);

point_t actionByAI(ChessData* data); // ����ִ������

