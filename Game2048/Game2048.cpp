// Game2048.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Controller.h"


int main()
{
	/*Board board;
	int testValue[4][4] = { \
	4,2,5,6,\
	3,1,2,1,\
	1,2,3,4,\
	1,2,3,4};
	vector<vector<Tile*>> tiles = board.getTiles();
	for (int row = 0; row < 4; row++) {
	for (int col = 0; col < 4; col++) {
	tiles[row][col]->setBlock(new Block(testValue[row][col]));
	}
	}
	for (int row = 0; row < 4; row++) {
	for (int col = 0; col < 4; col++) {
	Block* block = tiles[row][col]->getBlock();
	if (block == NULL) {
	cout << "0 ";
	}
	else {
	cout << block->getNumber() << " ";
	}
	}
	cout << endl;
	}
	cout << "board.getMono() : " << board.getMono() << endl;
	cout << "board.getSmoothness() : " << board.getSmoothness() << endl;

	return 0;*/
	Controller::getInstance().start();
	Controller::getInstance().destroy();
	return 0;
}
