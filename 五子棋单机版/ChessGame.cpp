#include "ChessGame.h"

ChessGame::ChessGame(Man* man, AI* ai, Chess* chess):man(man),ai(ai),chess(chess)
{

	man->init(chess);
	ai->init(chess);
}

//对局开始游戏
void ChessGame::play()
{
	chess->init();
	while (1) {
		man->go();
		if (chess->checkOver()) {
			chess->init();
			continue;
		}
		ai->go();
		if (chess->checkOver()) {
			chess->init();
			continue;
		}
	}
}
