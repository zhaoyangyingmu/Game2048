#pragma once
#ifndef BOARD_H
#define BOARD_H
class Board;
#include "Controller.h"
#include "ObserverPattern.h"
#include "Tile.h"

class Board : public Observable //继承被观察者
{
	static const int COL_NUMBER;
	static const int ROW_NUMBER;
	bool playerTurn;// 轮电脑下还是玩家下
	int credit; // 总分
	int destroyLimit; // 消除方块道具的次数限制
	int randomLimit; // 打乱的次数限制
	vector<vector<Tile *>> tiles; // 棋盘
	Player* player; // 玩家

					// 为移动设置的方法
	bool join(Block* block, Direction dir, bool simulate); // 合并
	bool move(Block* block, Direction dir, bool simulate); // 移动
	bool slide(Direction dir, bool simulate); // 滑动
	void freshAllBlock(); // 将block的状态更新

	int getRandNumber(int range); // 获取随机数

	void addCredit(int increment) { // 增加分数
		credit += increment;
	}

public:
	vector<vector<Tile *>> getTiles() { //获取棋盘格子
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

	Tile* getTileByDirection(const Tile* tile, Direction dir) const; //通过方向获取棋盘
	Tile* getTileByBlock(Block* block); // 通过滑块获取棋盘

	vector<Tile*> getEmptyTiles(); // 获取空的棋盘数组
	bool setBlockByRandom(); // 随机设置一个块
	bool setBlock(int idx, int number); // 在指定位置，设定指定大小的块

										// tools
	void destroyMinBlock(); // 销毁最小滑块
	void randomizeBlocks(); // 打乱棋盘
	int getDestroyLimit() const { // 获得消除道具数量
		return destroyLimit;
	}
	int getRandomLimit() const { // 获取打乱道具数量
		return randomLimit;
	}

	int getCredit() const { // 获取分值
		return credit;
	}

	friend class ConsoleView;

	// for AI
	double getSmoothness() const; // 得到平滑估值
	double getMax() const; // 得到最大数值
	double getMono() const; // 得到单调性
	Direction tranform(int i) const; // 获取对应的方向


	bool playerAct(Direction dir); //玩家移动
	bool computerAct(); // 电脑移动
	bool getPlayerTurn() const { // 得到次序
		return playerTurn;
	}
	void setPlayerTurn(bool turn) { //设置次序
		playerTurn = turn;
	}
};

#endif
