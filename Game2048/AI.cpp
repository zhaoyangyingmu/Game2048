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
		// AI���ֽ������ͬsmoothWeight��ʹ�õ��ߴ���Խ�࣬˵��AIЧ��Խ���
		// 1.0 ʹ�õ��� 5 �Σ� 
		// 0.5 ʹ�õ��� 5�� ��
		// 1.5ʹ�õ��� 0�Σ�˵��1.5��� ��
		// 2.0 ʹ�õ��� 6�� ��
		monoWeight = 1.0, // ������
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
	 * ģ���˺����̶��ĵ��龰��
	 * �˻�������������ƶ����ı����̾��ƣ�
	 * �����������������ֵ����ӣ��ı����̾��ơ�
	 */
	if (board.getPlayerTurn()) { // find max
		double bestValue = min; // ���������
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
		if (size == 0) { // û�����ˣ�˵���Է�����֮��ȫ�����ˣ�����û�����������
			cout << "computer turn, size == 0 " << endl;
		}
		for (int i = 0; i < size; i++) { // 2
			Board newBoard = board;
			newBoard.setBlock(i, 2); // ÿ���յ�
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
	double bestValue = min; // �����ѡ��һ��������������
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