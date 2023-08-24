#include<iostream>
#include"ChessGame.h"

using namespace std;

int main() {
	Man man;
	AI ai;
	//Chess chess;
	Chess chess(19,50,50,51);
	ChessGame game(&man ,&ai,&chess);

	game.play();

	return 0;
}