#include "stdafx.h"
#include "AI.h"
#include <cmath>


AI::AI()
{
}


AI::~AI()
{
}

double AI::evaluate(Board& board) const {
	double smoothWeight = 1.5, 
		// AI表现结果（不同smoothWeight，使用道具次数越多，说明AI效果越差）：
		// 1.0 使用道具 5 次； 
		// 0.5 使用道具 5次 ；
		// 1.5使用道具 0次，说明1.5最佳 ；
		// 2.0 使用道具 6次 。
		monoWeight = 1.0, // 单调性
		emptyWeight = 2.7,
		maxWeight = 1.0;
	double emptyNumber = board.getEmptyTiles().size();

	double smoothPart = board.getSmoothness() * smoothWeight;
	double monoPart = board.getMono() * monoWeight;
	double emptyPart = log(emptyNumber) * emptyWeight;
	double maxPart = board.getMax() * maxWeight;
	double result = smoothPart
		+ monoPart
		+ emptyPart
		+ maxPart;
	//cout << "evaluation: " << result << endl;
	return result;
}

double AI::minimax(int depth, Board& board, double min, double max)
{
	if (depth == 0) {
		return evaluate(board);
	}
	/**
	 * 模拟人和棋盘对弈的情景：
	 * 人会进行上下左右移动，改变棋盘局势；
	 * 而机器，则会进行数字的增加，改变棋盘局势。
	 */
	if (board.getPlayerTurn()) { // find max
		double bestValue = min; // 做好最坏打算
		for (int i = 0; i < 4; i++) {
			Board newBoard = board;
			Direction dir = board.tranform(i);
			bool result = newBoard.playerAct(dir);
			if (!result) { // can't move in this direction
				continue;
			}
			double tmpValue = minimax(depth - 1, newBoard, bestValue, max);
			if (tmpValue > bestValue) {
				bestValue = tmpValue;
			}
			if (bestValue >= max) {
				return max;
			}
		}
		return bestValue;
	}
	else { // find min
		double bestValue = max;
		vector<Tile*> emptyTiles = board.getEmptyTiles();
		int size = emptyTiles.size();
		if (size == 0) { // 没法动了，说明对方动完之后，全部满了，不过没有这种情况。
			cout << "computer turn, size == 0 " << endl;
		}
		for (int i = 0; i < size; i++) { // 2
			Board newBoard = board;
			newBoard.setBlock(i, 2); // 每个空的
			double tmpValue = minimax(depth - 1, newBoard, min, bestValue);
			if (tmpValue < bestValue) {
				bestValue = tmpValue;
			}
			if (bestValue <= min) {
				return min;
			}
		}
		for (int i = 0; i < size; i++) { // 4
			Board newBoard = board;
			newBoard.setBlock(i, 4);
			double tmpValue = minimax(depth - 1, newBoard, min, bestValue);
			if (tmpValue < bestValue) {
				bestValue = tmpValue;
			}
			if (bestValue <= min) {
				return min;
			}
		}
		return bestValue;
	}
}

Direction AI::findBest(const Board& board) {
	int depth = 5;
	double max = 10000;
	double min = -10000;
	double bestValue = min; // 先随机选择一个，进行评估。
	Direction bestDir = Direction::UP;
	for (int i = 0; i < 4; i++) {
		Board newBoard = board;
		Direction dir = board.tranform(i);
		bool result = newBoard.playerAct(dir);
		if (!result) { // can't move in this direction
			continue;
		}
		double tmpValue = minimax(depth - 1, newBoard, bestValue, max);
		if (tmpValue > bestValue) {
			bestValue = tmpValue;
			bestDir = dir;
		}
	}
	return bestDir;
}