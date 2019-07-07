#pragma once
#ifndef BOARD_H
#define BOARD_H
class Board;
#include "Controller.h"
#include "ObserverPattern.h"
#include "Tile.h"

class Board : public Observable //�̳б��۲���
{
	static const int COL_NUMBER;
	static const int ROW_NUMBER;
	bool playerTurn;// �ֵ����»��������
	int credit; // �ܷ�
	int destroyLimit; // ����������ߵĴ�������
	int randomLimit; // ���ҵĴ�������
	vector<vector<Tile *>> tiles; // ����
	Player* player; // ���

					// Ϊ�ƶ����õķ���
	bool join(Block* block, Direction dir, bool simulate); // �ϲ�
	bool move(Block* block, Direction dir, bool simulate); // �ƶ�
	bool slide(Direction dir, bool simulate); // ����
	void freshAllBlock(); // ��block��״̬����

	int getRandNumber(int range); // ��ȡ�����

	void addCredit(int increment) { // ���ӷ���
		credit += increment;
	}

public:
	vector<vector<Tile *>> getTiles() { //��ȡ���̸���
		return tiles;
	};
	Board();
	Board(const Board& board);
	~Board();
	Player* getPlayer() {
		return player;
	}
	void restart();
	bool checkWin();
	bool checkLose();

	//for debug
	void cheat();
	void setDead();

	Tile* getTileByDirection(const Tile* tile, Direction dir) const; //ͨ�������ȡ����
	Tile* getTileByBlock(Block* block); // ͨ�������ȡ����

	vector<Tile*> getEmptyTiles(); // ��ȡ�յ���������
	bool setBlockByRandom(); // �������һ����
	bool setBlock(int idx, int number); // ��ָ��λ�ã��趨ָ����С�Ŀ�

										// tools
	void destroyMinBlock(); // ������С����
	void randomizeBlocks(); // ��������
	int getDestroyLimit() const { // ���������������
		return destroyLimit;
	}
	int getRandomLimit() const { // ��ȡ���ҵ�������
		return randomLimit;
	}

	int getCredit() const { // ��ȡ��ֵ
		return credit;
	}

	friend class ConsoleView;

	// for AI
	double getSmoothness() const; // �õ�ƽ����ֵ
	double getMax() const; // �õ������ֵ
	double getMono() const; // �õ�������
	Direction tranform(int i) const; // ��ȡ��Ӧ�ķ���


	bool playerAct(Direction dir); //����ƶ�
	bool computerAct(); // �����ƶ�
	bool getPlayerTurn() const { // �õ�����
		return playerTurn;
	}
	void setPlayerTurn(bool turn) { //���ô���
		playerTurn = turn;
	}
};

#endif
