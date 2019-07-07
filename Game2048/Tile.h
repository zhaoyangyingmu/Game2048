#pragma once
class Block
{
	bool joinStatus; // �����������ǰ��״̬Ϊtrue����ʾ���Ժϲ���
	                 // ����������������Ѿ�������һ�κϲ�����ô����Ϊfalse����ֹ���������ĺϲ���
					// �� 2��2��2��2�����ң����ϲ���4��4��������8��
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
	// �ϲ�֮���������֡�
	void doubleNumber() {
		number *= 2;
	}
};

/**
 * Tile �ǹ������̵ķ��񣬶�Block�Ƿ����е�Ԫ�أ���2��4��8��16��
 * Tile�е�Block*ָ�򷽸�Ԫ�أ�Block* ����Ϊ�գ���Ϊ��Щ�ط���һ�������ɷ���
 * BlockԪ����Board���д��������١�
 */
class Tile
{
	// row��col�����ŷ����������ϵ�λ�á�
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
