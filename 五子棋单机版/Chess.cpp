#include "Chess.h"
#include<graphics.h>
#include<conio.h>
#pragma comment(lib,"winmm.lib")
#include <mmsystem.h> // ���� Windows Multimedia API ͷ�ļ�
#include<windows.h>
void putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
	// ������ʼ��
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}




Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
    :gradeSize(gradeSize),margin_x(marginX),margin_y(marginY),chessSize(chessSize)
{
    //Ĭ�Ϻ�������
    playerFlag = CHESS_BLACK;
	lastlastDownPos.col = -1;
	lastlastDownPos.row = -1;
    //��ʼ�����̣�vector������
    for (int i = 0; i < gradeSize; i++) {
        vector<int>row;
        for (int j = 0; j < gradeSize; j++) {
            row.push_back(0);
        }
        chessMap.push_back(row);
    }
}

void Chess::init()
{
    //������Ϸ����
    initgraph(1024,1024, EX_SHOWCONSOLE);

    //��ʾ����
    loadimage(0, L"res/����.jpg");

	mciSendString(L"play res/start.WAV",0,0,0);

    //���غ������ͼƬ
    loadimage(&chessBlackImg, L"res/black.png",chessSize,chessSize,true);
    loadimage(&chessWhiteImg, L"res/white.png", chessSize, chessSize, true);
	loadimage(&lastChessDownPosImg,L"res/lastDown.png", chessSize, chessSize, true);
    //��������
    for (int i = 0; i < gradeSize; i++) {
        for (int j = 0; j < gradeSize; j++) {
            chessMap[i][j] = 0;
        }
    }

}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	int col = (x - margin_x) / chessSize;
	int row = (y - margin_y) / chessSize;

	int leftTopPosX = margin_x + chessSize * col;
	int leftTopPosY = margin_y + chessSize * row;
	int offset = chessSize * 0.4; // 20 �������ģ����������

	int len;
	int selectPos = false;

	do {
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			pos->row = row;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������Ͻǵľ���
		len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			pos->row = row;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������½ǵľ���
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������½ǵľ���
		len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col + 1;

			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}
	} while (0);

	return selectPos;
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
	int x = margin_x + chessSize * pos->col - 0.5 * chessSize +7;
	int y = margin_y + chessSize * pos->row - 0.5 * chessSize+6;
	if (kind == CHESS_WHITE) {
		putimagePNG(x, y, &chessWhiteImg);
	}
	else {
		putimagePNG(x, y, &chessBlackImg);
	}
	mciSendString(L"play res/down8.wav", 0, 0, 0);
	updateChessMap(pos);

	putimagePNG(x, y, &lastChessDownPosImg);
	lastlastDownPos = *pos;
	//if (lastlastDownPos.col != -1) {
	//	if (kind == CHESS_WHITE) {
	//		putimagePNG(lastlastDownPos.col, lastlastDownPos.row, &chessBlackImg);
	//	}
	//	else {
	//		putimagePNG(lastlastDownPos.col, lastlastDownPos.row, &chessWhiteImg);
	//	}
	//}


}

int Chess::getGradeSize()
{
    return gradeSize;
}

int Chess::getChessData(ChessPos* pos)
{
    return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
    return chessMap[row][col];
}

bool Chess::checkOver()
{
	if (checkWin()) {
		if (playerFlag == false) {//�����Ǽ٣����Ǹð������ˣ���ô���Ǻ���Ӯ��
			loadimage(0, L"res/victory.jpg");
			mciSendString(L"play res/����ʤ��.mp3", 0, 0, 0);
		}
		else {//������true����ô���ǰ���Ӯ��
			loadimage(0, L"res/loser.jpg");
			mciSendString(L"play res/ʧ��.mp3", 0, 0, 0);
		}
		std::cout << (playerFlag == false ? "���ʤ��" : "���ʧ�ܣ�AIʤ��") << std::endl;
		_getch();	//��ͣ�������������
		return true;
	}
    return false;
}

void Chess::updateChessMap(ChessPos* pos)
{
	chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
	playerFlag = !playerFlag;
	lastPos = *pos;
}

bool Chess::checkWin()
{
	int row = lastPos.row;
	int col = lastPos.col;


	//����ˮƽ����
	for (int i = 0; i < 5; i++) {
		//���ڵ�i�Σ���������������λ�ã�
		if (col -i>=0&&
			col-i+4<gradeSize&&
			chessMap[row][col - i] == chessMap[row][col - i + 1] && 
			chessMap[row][col - i] == chessMap[row][col - i + 2] && 
			chessMap[row][col - i] == chessMap[row][col - i + 3] && 
			chessMap[row][col - i] == chessMap[row][col - i + 4]) {
			//�Ѿ�������������
			return true;
		}
	}

	//���Ӵ�ֱ����
	for (int i = 0; i < 5; i++) {
		//���ڵ�i�Σ���������������λ�ã�
		if (row - i >= 0 &&
			row - i + 4 < gradeSize &&
			chessMap[row-i][col] == chessMap[row - i + 1][col] &&
			chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i +3][col] &&
			chessMap[row - i][col] == chessMap[row - i + 4][col]) {
			//�Ѿ�������������
			return true;
		}
	}

	//������������'/'����
	for (int i = 0; i < 5; i++) {
		//���ڵ�i�Σ���������������λ�ã�
		if (row + i-4 >= 0 &&
			row +i < gradeSize &&
			col-i>=0 && col-i+4<gradeSize&&
			chessMap[row + i][col-i] == chessMap[row + i -1][col-i+1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col-i+2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col-i+3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col-i+4]) {
			//�Ѿ�������������
			return true;
		}
	}

	//�����������ϡ�\������
	for (int i = 0; i < 5; i++) {
		//���ڵ�i�Σ���������������λ�ã�
		if (row - i  >= 0 && row - i +4< gradeSize &&
			col - i >= 0 && col - i + 4 < gradeSize &&
			chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] &&
			chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] &&
			chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4]) {
			//�Ѿ�������������
			return true;
		}
	}



	return false;
}


