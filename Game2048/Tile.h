#pragma once
class Block
{
	bool joinStatus; // 玩家输入命令前，状态为true，表示可以合并；
	                 // 玩家输入命令后，如果已经进行了一次合并，那么设置为false，阻止进行连续的合并。
					// 如 2，2，2，2，向右，则会合并成4，4；而不是8。
	int number;
public:
	Block(int nb = 2) : joinStatus(true), number(nb) {}
	~Block();
	int getNumber() const {
		return number;
	}
	bool canJoin(int nb) const {
		return (nb == number) && joinStatus;
	}
	void setJoin(const bool& st) {
		joinStatus = st;
	}
	// 合并之后，扩大数字。
	void doubleNumber() {
		number *= 2;
	}
};

/**
 * Tile 是构成棋盘的方格，而Block是方格中的元素，如2，4，8，16。
 * Tile中的Block*指向方格元素，Block* 可以为空，因为有些地方不一定会生成方格。
 * Block元素有Board进行创建和销毁。
 */
class Tile
{
	// row和col标致着方格在棋盘上的位置。
	int row;
	int col;
	Block * block;
public:
	Tile(int rw, int cl) : row(rw), col(cl), block(NULL) {}
	~Tile();
	void setBlock(Block* blk);
	Block* getBlock() const {
		return block;
	}
	int getRow() const {
		return row;
	}

	int getCol() const {
		return col;
	}
};
