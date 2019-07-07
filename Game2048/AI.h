#pragma once
#ifndef AI_H
#define AI_H
class AI;
#include "Board.h"


class AI
{
	/**
	 * �ж����̵ľ��ƺû�
	 */
	double evaluate(Board& board) const;

	/**
	 * minmax�㷨
	 */
	double minimax(int depth, Board& board, double min, double max);
public:
	AI();
	~AI();
	/**
	 * �������̷�����ѷ���
	 */
	Direction findBest(const Board& board);
};
#endif // !AI_H