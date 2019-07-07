#pragma once
#ifndef AI_H
#define AI_H
class AI;
#include "Board.h"


class AI
{
	/**
	 * 判断棋盘的局势好坏
	 */
	double evaluate(Board& board) const;

	/**
	 * minmax算法
	 */
	double minimax(int depth, Board& board, double min, double max);
public:
	AI();
	~AI();
	/**
	 * 输入棋盘返回最佳方向
	 */
	Direction findBest(const Board& board);
};
#endif // !AI_H