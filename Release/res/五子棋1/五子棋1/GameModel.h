#pragma once
// ---- 五子棋游戏模型类 ---- //
#include <vector>

typedef enum {
    CHESS_WHITE = -1,
    CHESS_BLACK = 1
} chess_kind_t;

// 棋盘尺寸
const int margin_x = 44;//49;
const int margin_y = 43;// 49;

const int BOARD_GRAD_SIZE = 13;
const int MARGIN = 49; // 棋盘边缘空隙
const int CHESS_RADIUS = 15; // 棋子半径
const int MARK_SIZE = 20; // 落子标记边长
const float BLOCK_SIZE = 67.4; // 格子的大小
const int POS_OFFSET = BLOCK_SIZE * 0.4; // 20 鼠标点击的模糊距离上限
const int AI_THINK_TIME = 700; // AI下棋的思考时间

struct  ChessData {
    // 存储当前游戏棋盘和棋子的情况,空白为0，黑子1，白子-1
    int chessMap[BOARD_GRAD_SIZE][BOARD_GRAD_SIZE];

    // 存储各个点位的评分情况，作为AI下棋依据
    int scoreMap[BOARD_GRAD_SIZE][BOARD_GRAD_SIZE];

    // 标示下棋方, true:黑棋方  false: AI 白棋方（AI方）
    bool playerFlag;
};

typedef struct point {
    int row;
    int col;
} point_t;

void initChessData(ChessData*); // 开始游戏

void updateGameMap(ChessData* data, int row, int col);

bool checkWin(ChessData* game, int row, int col);

void calculateScore(ChessData* dat);

point_t actionByAI(ChessData* data); // 机器执行下棋

