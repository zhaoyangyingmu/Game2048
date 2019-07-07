#include "stdafx.h"
#include "Board.h"


/**
* return random number according to current time.
* value range is [0,range-1], value type: int.
*/

int Board::getRandNumber(int range) {
	srand((unsigned)time(NULL));
	int result = rand() % range;
	return result;
}

const int Board::ROW_NUMBER = 4;
const int Board::COL_NUMBER = 4;

Board::Board() : credit(0), destroyLimit(20), randomLimit(20), playerTurn(true)
{
	player = new Player();
	for (int i = 0; i < ROW_NUMBER; i++) {
		vector<Tile *> tileRow;
		tiles.push_back(tileRow);
		for (int j = 0; j < COL_NUMBER; j++) {
			tiles[i].push_back(new Tile(i, j));
		}
	}
	setBlockByRandom();
	setBlockByRandom();
}

Board::Board(const Board& board) {
	Board& boardOut = (Board&)board;
	credit = board.getCredit();
	destroyLimit = board.getDestroyLimit();
	randomLimit = board.getRandomLimit();
	playerTurn = board.getPlayerTurn();

	player = new Player();
	player->setName(boardOut.getPlayer()->getName());

	vector<vector<Tile *>> tilesOut = boardOut.getTiles();

	for (int i = 0; i < ROW_NUMBER; i++) {
		vector<Tile *> tileRow;
		tiles.push_back(tileRow);
		for (int j = 0; j < COL_NUMBER; j++) {
			Tile* tile = new Tile(i, j);
			tiles[i].push_back(tile);
			Block* block = tilesOut[i][j]->getBlock();
			if (block != NULL) {
				tiles[i][j]->setBlock(new Block(block->getNumber()));
			}

		}
	}
}

void Board::restart() {
	credit = 0;
	destroyLimit = 20;
	randomLimit = 20;
	for (int row = 0; row < ROW_NUMBER; row++) {
		for (int col = 0; col < COL_NUMBER; col++) {
			Block* block = tiles[row][col]->getBlock();
			if (block == NULL) {
				continue;
			}
			delete block;
			tiles[row][col]->setBlock(NULL);
		}
	}
	setBlockByRandom();
	setBlockByRandom();
}


Board::~Board()
{
	delete player;
	for (int i = 0; i < ROW_NUMBER; i++) {
		for (int j = 0; j < COL_NUMBER; j++) {
			Block* block = tiles[i][j]->getBlock();
			if (!block) {
				delete block;
			}
			delete tiles[i][j];
		}
	}
}


bool Board::checkWin()
{
	for (int row = 0; row < ROW_NUMBER; row++)
	{
		for (int col = 0; col < COL_NUMBER; col++)
		{
			Block* block = tiles[row][col]->getBlock();
			if (block == NULL) {
				continue;
			}
			if (block->getNumber() == 2048)
			{
				return true;
			}
		}
	}
	return false;
}

bool Board::checkLose()
{
	// 输的情况，道具用完了， 并且不能再走了。
	if (destroyLimit > 0 || randomLimit > 0) {
		return false;
	}
	// 如果有一个方向还能动，就没有输，return false;
	if (slide(Direction::UP, true)) {
		return false;
	}
	if (slide(Direction::DOWN, true)) {
		return false;
	}
	if (slide(Direction::LEFT, true)) {
		return false;
	}
	if (slide(Direction::RIGHT, true)) {
		return false;
	}
	return true;
}

void Board::cheat() {
	for (int col = 0; col < 2; col++) {
		Block* block = tiles[0][col]->getBlock();
		if (block != NULL) {
			delete block;
		}
		tiles[0][col]->setBlock(new Block(1024));
	}
}

void Board::setDead() {
	randomLimit = 1;
	destroyLimit = 0;
	for (int row = 0; row < ROW_NUMBER; row++) {
		for (int col = 0; col < COL_NUMBER; col++) {
			Block* block = tiles[row][col]->getBlock();
			if (!block) {
				delete block;
			}
			int number = (row * 10 + col);
			if (number == 0) {
				tiles[row][col]->setBlock(NULL);
				continue;
			}
			tiles[row][col]->setBlock(new Block(number));
		}
	}
}

Tile* Board::getTileByDirection(const Tile* tile, Direction dir) const
{
	switch (dir) {
	case Direction::UP:
		if (tile->getRow() > 0) {
			return tiles[tile->getRow() - 1][tile->getCol()];
		}
		break;

	case Direction::DOWN:
		if (tile->getRow() < ROW_NUMBER) {
			return tiles[tile->getRow() + 1][tile->getCol()];
		}
		break;

	case Direction::RIGHT:
		if (tile->getCol() < COL_NUMBER) {
			return tiles[tile->getRow()][tile->getCol() + 1];
		}
		break;

	case Direction::LEFT:
		if (tile->getCol() > 0) {
			return tiles[tile->getRow()][tile->getCol() - 1];
		}
		break;
	default:
		return NULL;
	}
	return NULL;

}

Tile* Board::getTileByBlock(Block* block) {
	for (int row = 0; row < ROW_NUMBER; row++) {
		for (int col = 0; col < COL_NUMBER; col++) {
			if (tiles[row][col]->getBlock() == block) {
				return tiles[row][col];
			}
		}
	}
	return NULL;
}

// 加上 simulate ， simulate时，能动不动，返回true。
bool Board::join(Block* block, Direction dir, bool simulate) {
	// join 成功返回true, 否则返回false
	// 意味着给定block， 向着目标方向合并
	Tile* tile = getTileByBlock(block);
	int tileRow = tile->getRow();
	int tileCol = tile->getCol();
	switch (dir) {
	case Direction::LEFT:
		for (int col = tileCol - 1; col >= 0; col--) {
			Block * targetBlock = tiles[tileRow][col]->getBlock();
			if (targetBlock == NULL) {
				continue;
			}
			if (targetBlock->canJoin(block->getNumber())) {
				// 直接就Join吧
				if (!simulate) {
					targetBlock->doubleNumber();
					targetBlock->setJoin(false);
					tile->setBlock(NULL);
					delete block;
					// 还需要加以下总分
					addCredit(targetBlock->getNumber());
				}
				return true;
			}
			else {
				return false;
			}
		}
		break;
	case Direction::RIGHT:
		for (int col = tileCol + 1; col < COL_NUMBER; col++) {
			Block * targetBlock = tiles[tileRow][col]->getBlock();
			if (targetBlock == NULL) {
				continue;
			}
			if (targetBlock->canJoin(block->getNumber())) {
				if (!simulate) {
					targetBlock->doubleNumber();
					targetBlock->setJoin(false);
					tile->setBlock(NULL);
					delete block;
					// 还需要加以下总分
					addCredit(targetBlock->getNumber());
				}
				return true;
			}
			else {
				return false;
			}
		}

		break;
	case Direction::UP:
		for (int row = tileRow - 1; row >= 0; row--) {
			Block * targetBlock = tiles[row][tileCol]->getBlock();
			if (targetBlock == NULL) {
				continue;
			}
			if (targetBlock->canJoin(block->getNumber())) {
				if (!simulate) {
					targetBlock->doubleNumber();
					targetBlock->setJoin(false);
					tile->setBlock(NULL);
					delete block;
					// 还需要加以下总分
					addCredit(targetBlock->getNumber());
				}
				return true;
			}
			else {
				return false;
			}
		}

		break;
	case Direction::DOWN:
		for (int row = tileRow + 1; row < ROW_NUMBER; row++) {
			Block * targetBlock = tiles[row][tileCol]->getBlock();
			if (targetBlock == NULL) {
				continue;
			}
			if (targetBlock->canJoin(block->getNumber())) {
				if (!simulate) {
					targetBlock->doubleNumber();
					targetBlock->setJoin(false);
					tile->setBlock(NULL);
					delete block;
					// 还需要加以下总分
					addCredit(targetBlock->getNumber());
				}
				return true;
			}
			else {
				return false;
			}
		}
		break;
	default:
		return false;
	}
	return false;
}

// 加上 simulate ， simulate时，能动不动，返回true。
bool Board::move(Block* block, Direction dir, bool simulate) {
	// move 成功返回true, 否则返回false
	Tile* tile = getTileByBlock(block);
	int tileRow = tile->getRow();
	int tileCol = tile->getCol();
	int targetRow = -1;
	int targetCol = -1;
	switch (dir) {
	case Direction::LEFT:
		if (tileCol - 1 < 0) {
			return false;
		}
		for (int col = tileCol - 1; col >= 0; col--) {
			Block * targetBlock = tiles[tileRow][col]->getBlock();
			if (targetBlock == NULL) {
				targetRow = tileRow;
				targetCol = col;
				continue;
			}
			else {
				if (targetRow != -1 && targetCol != -1) {
					if (!simulate) {
						tiles[targetRow][targetCol]->setBlock(block);
						tile->setBlock(NULL);
					}
					return true;
				}
				else {
					return false;
				}
			}
		}
		if (targetRow != -1 && targetCol != -1) {
			if (!simulate) {
				tiles[targetRow][targetCol]->setBlock(block);
				tile->setBlock(NULL);
			}
			return true;
		}
		else {
			return false;
		}
		break;
	case Direction::RIGHT:
		if (tileCol + 1 >= COL_NUMBER) {
			return false;
		}
		for (int col = tileCol + 1; col < COL_NUMBER; col++) {
			Block * targetBlock = tiles[tileRow][col]->getBlock();
			if (targetBlock == NULL) {
				targetRow = tileRow;
				targetCol = col;
				continue;
			}
			else {
				if (targetRow != -1 && targetCol != -1) {
					if (!simulate) {
						tiles[targetRow][targetCol]->setBlock(block);
						tile->setBlock(NULL);
					}
					return true;
				}
				else {
					return false;
				}
			}
		}
		if (targetRow != -1 && targetCol != -1) {
			if (!simulate) {
				tiles[targetRow][targetCol]->setBlock(block);
				tile->setBlock(NULL);
			}
			return true;
		}
		else {
			return false;
		}
		break;
	case Direction::UP:
		if (tileRow - 1 < 0) {
			return false;
		}
		for (int row = tileRow - 1; row >= 0; row--) {
			Block * targetBlock = tiles[row][tileCol]->getBlock();
			if (targetBlock == NULL) {
				targetRow = row;
				targetCol = tileCol;
				continue;
			}
			else {
				if (targetRow != -1 && targetCol != -1) {
					if (!simulate) {
						tiles[targetRow][targetCol]->setBlock(block);
						tile->setBlock(NULL);
					}
					return true;
				}
				else {
					return false;
				}
			}
		}
		if (targetRow != -1 && targetCol != -1) {
			if (!simulate) {
				tiles[targetRow][targetCol]->setBlock(block);
				tile->setBlock(NULL);
			}
			return true;
		}
		else {
			return false;
		}
		break;
	case Direction::DOWN:
		if (tileRow + 1 >= ROW_NUMBER) {
			return false;
		}
		for (int row = tileRow + 1; row < ROW_NUMBER; row++) {
			Block * targetBlock = tiles[row][tileCol]->getBlock();
			if (targetBlock == NULL) {
				targetRow = row;
				targetCol = tileCol;
				continue;
			}
			else {
				if (targetRow != -1 && targetCol != -1) {
					if (!simulate) {
						tiles[targetRow][targetCol]->setBlock(block);
						tile->setBlock(NULL);
					}
					return true;
				}
				else {
					return false;
				}
			}
		}
		if (targetRow != -1 && targetCol != -1) {
			if (!simulate) {
				tiles[targetRow][targetCol]->setBlock(block);
				tile->setBlock(NULL);
			}
			return true;
		}
		else {
			return false;
		}
		break;
	default:
		return false;
	}
}

// 加上 simulate ， simulate时，能动不动，返回true。
bool Board::slide(Direction dir, bool simulate) {
	bool alreadySlide = false;
	switch (dir) {
	case Direction::LEFT: // 向左边滑动 <-, 应该从左边第二个开始循环，外层是行，都要遍历
		for (int row = 0; row < ROW_NUMBER; row++) {
			for (int col = 1; col < COL_NUMBER; col++) {
				Block* block = tiles[row][col]->getBlock();
				if (block == NULL) {
					continue;
				}
				else {
					if (simulate) {
						if (join(block, dir, simulate)) {
							return true;
						}
						if (move(block, dir, simulate)) {
							return true;
						}
					}
					else {
						if (join(block, dir, simulate)) {
							alreadySlide = true;
							continue;
						}
						if (move(block, dir, simulate)) {
							alreadySlide = true;
							continue;
						}
					}
				}
			}
		}
		if (simulate) {
			return false;
		}
		else {
			return alreadySlide;
		}
		break;
	case Direction::RIGHT: // 向右边滑动 -> ， 应该从右边第二个开始循环
		for (int row = 0; row < ROW_NUMBER; row++) {
			for (int col = COL_NUMBER - 2; col >= 0; col--) {
				Block* block = tiles[row][col]->getBlock();
				if (block == NULL) {
					continue;
				}
				else {
					// can join?
					if (simulate) {
						if (join(block, dir, simulate)) {
							return true;
						}
						if (move(block, dir, simulate)) {
							return true;
						}
					}
					else {
						if (join(block, dir, simulate)) {
							alreadySlide = true;
							continue;
						}
						if (move(block, dir, simulate)) {
							alreadySlide = true;
							continue;
						}
					}
				}
			}
		}
		if (simulate) {
			return false;
		}
		else {
			return alreadySlide;
		}
		break;
	case Direction::UP:
		for (int col = 0; col < COL_NUMBER; col++) {
			for (int row = 1; row < ROW_NUMBER; row++) {
				Block* block = tiles[row][col]->getBlock();
				if (block == NULL) {
					continue;
				}
				else {
					// can join?
					if (simulate) {
						if (join(block, dir, simulate)) {
							return true;
						}
						if (move(block, dir, simulate)) {
							return true;
						}
					}
					else {
						if (join(block, dir, simulate)) {
							alreadySlide = true;
							continue;
						}
						if (move(block, dir, simulate)) {
							alreadySlide = true;
							continue;
						}
					}
				}
			}
		}
		if (simulate) {
			return false;
		}
		else {
			return alreadySlide;
		}
		break;
	case Direction::DOWN:
		for (int col = 0; col < COL_NUMBER; col++) {
			for (int row = ROW_NUMBER - 2; row >= 0; row--) {
				Block* block = tiles[row][col]->getBlock();
				if (block == NULL) {
					continue;
				}
				else {
					// can join?
					if (simulate) {
						if (join(block, dir, simulate)) {
							return true;
						}
						if (move(block, dir, simulate)) {
							return true;
						}
					}
					else {
						if (join(block, dir, simulate)) {
							alreadySlide = true;
							continue;
						}
						if (move(block, dir, simulate)) {
							alreadySlide = true;
							continue;
						}
					}
				}
			}
		}
		if (simulate) {
			return false;
		}
		else {
			return alreadySlide;
		}
		break;
	}
	return alreadySlide;
}

void Board::freshAllBlock() {
	for (int row = 0; row < ROW_NUMBER; row++) {
		for (int col = 0; col < COL_NUMBER; col++) {
			Block* block = tiles[row][col]->getBlock();
			if (block == NULL) {
				continue;
			}
			else {
				block->setJoin(true);
			}
		}
	}
}

bool Board::playerAct(Direction dir)
{
	bool result = this->slide(dir, false);
	if (result) {
		playerTurn = false;
	}
	this->freshAllBlock();
	return result;
}

bool Board::computerAct() {
	vector<Tile *> emptyTiles = getEmptyTiles();
	if (emptyTiles.size() == 0) {
		return false;
	}
	playerTurn = true;
	int randIdx = getRandNumber(emptyTiles.size());
	int number = randIdx % 2 == 0 ? 2 : 4;
	emptyTiles[randIdx]->setBlock(new Block(number));
	return true;
}


/**
* 相邻格子的数字相差越小，平滑性越好。
*/
double Board::getSmoothness() const {
	double smoothness = 0.0;
	for (int row = 0; row < ROW_NUMBER; row++) {
		for (int col = 0; col < COL_NUMBER; col++) {
			Block* block = tiles[row][col]->getBlock();
			if (block != NULL) {
				double value = log((double)(block->getNumber())) / log(2.0);
				// Direction: Right, Down
				for (int targetCol = col + 1; targetCol < COL_NUMBER; targetCol++) {
					Block* targetBlock = tiles[row][targetCol]->getBlock();
					if (targetBlock == NULL) {
						continue;
					}
					double targetValue = log((double)(targetBlock->getNumber())) / log(2.0);
					double absence = value > targetValue ? (value - targetValue) : (targetValue - value);
					smoothness -= absence;
					break;
				}
				for (int targetRow = row + 1; targetRow < ROW_NUMBER; targetRow++) {
					Block* targetBlock = tiles[targetRow][col]->getBlock();
					if (targetBlock == NULL) {
						continue;
					}
					double targetValue = log((double)(targetBlock->getNumber())) / log(2.0);
					double absence = value > targetValue ? (value - targetValue) : (targetValue - value);
					smoothness -= absence;
					break;
				}
			}
		}
	}
	return smoothness;
}

/**
* 最大值越大，越容易拼出2048，局势越好
*/
double Board::getMax() const {
	int max = 0;
	for (int row = 0; row < ROW_NUMBER; row++) {
		for (int col = 0; col < COL_NUMBER; col++) {
			Block* block = tiles[row][col]->getBlock();
			if (block == NULL) {
				continue;
			}
			if (block->getNumber() > max) {
				max = block->getNumber();
			}
		}
	}
	return log(max) / log(2);
}

/**
* 单调性
* 数值越大，单调性越好
* 从上到下，递减
* 从上到下，递增
* 从左到右， 递减
* 从左到右， 递增
*/

double Board::getMono() const
{
	vector<double> monos;
	for (int i = 0; i < 4; i++) {
		monos.push_back(0.0);
	}

	// 从上到下
	for (int col = 0; col < COL_NUMBER; col++) { // 每一列
		for (int row = 0; row < ROW_NUMBER; row++) {
			Block* currentBlock = tiles[row][col]->getBlock();
			double currentValue = currentBlock == NULL ? (0) : (log(currentBlock->getNumber()) / log(2));
			double targetValue = 0.0;
			for (int targetRow = row + 1; targetRow < ROW_NUMBER; targetRow++) {
				Block* nextBlock = tiles[targetRow][col]->getBlock();
				if (nextBlock != NULL) {
					targetValue = (log((double)(nextBlock->getNumber())) / log(2));
					break;
				}
			}
			if (currentValue > targetValue) { // 递减
				monos[0] -= (currentValue - targetValue);
			}
			if (currentValue < targetValue) {
				monos[1] -= (targetValue - currentValue);
			}
		}
	}
	// 从左边到右边
	for (int row = 0; row < ROW_NUMBER; row++) {// 每一行
		for (int col = 0; col < COL_NUMBER; col++) {
			Block* currentBlock = tiles[row][col]->getBlock();
			double currentValue = currentBlock == NULL ? (0) : (log(currentBlock->getNumber()) / log(2));
			double targetValue = 0.0;
			for (int targetCol = col + 1; targetCol < COL_NUMBER; targetCol++) {
				Block* nextBlock = tiles[row][targetCol]->getBlock();
				if (nextBlock != NULL) {
					targetValue = (log((double)(nextBlock->getNumber())) / log(2));
					break;
				}
			}
			if (currentValue > targetValue) { // 递减
				monos[2] -= (currentValue - targetValue);
			}
			if (currentValue < targetValue) {
				monos[3] -= (targetValue - currentValue);
			}
		}
	}
	double monoCol = monos[0] > monos[1] ? monos[0] : monos[1];
	double monoRow = monos[2] > monos[3] ? monos[2] : monos[3];

	return monoCol + monoRow;
}

Direction Board::tranform(int i) const {
	switch (i) {
	case 0:
		return Direction::UP;
	case 1:
		return Direction::RIGHT;
	case 2:
		return Direction::DOWN;
	case 3:
		return Direction::LEFT;
	default:
		return Direction::UP;
	}
	return Direction::UP;
}



vector<Tile*> Board::getEmptyTiles()
{
	vector<Tile *> emptyTiles;
	for (int row = 0; row < ROW_NUMBER; row++) {
		for (int col = 0; col < COL_NUMBER; col++) {
			if (tiles[row][col]->getBlock() == NULL) {
				emptyTiles.push_back(tiles[row][col]);
			}
		}
	}
	return emptyTiles;
}


/**
* this method get empty tiles , then set one block to them by random.
* if success, return true;
* else, return false;
*/
bool Board::setBlockByRandom()
{
	vector<Tile *> emptyTiles = getEmptyTiles();
	if (emptyTiles.size() == 0) {
		return false;
	}
	int randIdx = getRandNumber(emptyTiles.size());
	emptyTiles[randIdx]->setBlock(new Block());
	return true;
}

// 这个方法是专门为AI设计的，其他地方禁止调用
// 如果成功了，自动设置playerTurn == true;
bool Board::setBlock(int idx, int number) {
	vector<Tile *> emptyTiles = getEmptyTiles();
	if (emptyTiles.size() == 0) {
		return false;
	}
	playerTurn = true;
	emptyTiles[idx]->setBlock(new Block(number));
	return true;
}

void Board::destroyMinBlock()
{
	// destroy 要有次数限制。
	if (destroyLimit <= 0) {
		return;
	}
	Tile* targetTile = NULL;
	Block* targetBlock = NULL;
	int minNum = 1000000;
	int nonemptyCount = 0;
	for (int row = 0; row < ROW_NUMBER; row++) {
		for (int col = 0; col < COL_NUMBER; col++) {
			Block* block = tiles[row][col]->getBlock();
			if (block == NULL) {
				continue;
			}
			else {
				nonemptyCount++;
			}
			if (block->getNumber() < minNum) {
				minNum = block->getNumber();
				targetTile = tiles[row][col];
				targetBlock = block;
			}
		}
	}

	if (targetTile == NULL) {
		// 说明全部为空
		return;
	}
	if (nonemptyCount == 1) {
		cout << "Since there is only one left, you can't destory any more. " << endl;
		return;
	}
	destroyLimit--;
	targetTile->setBlock(NULL);
	delete targetBlock;

}

/**
* 用稍微修改过的洗牌算法实现随机化，使得该block不会回到原来的tile中，
* 并且在每个位置上出现的概率都是 1 / (n-1)
*/
void Board::randomizeBlocks() {
	if (randomLimit <= 0) {
		return;
	}
	randomLimit--;
	int total = ROW_NUMBER * COL_NUMBER;
	for (int i = 0; i < (total - 1); i++) {//最后一个就不用打乱了
		srand((unsigned)time(NULL));
		int targetIdx = (rand() % (total - i - 1)) + (i + 1); // 0 ~ n-2 + 1
		int targetRow = targetIdx / COL_NUMBER;
		int targetCol = targetIdx % COL_NUMBER;
		int sourceRow = i / COL_NUMBER;
		int sourceCol = i % COL_NUMBER;
		Block* tmpBlock = tiles[sourceRow][sourceCol]->getBlock();
		tiles[sourceRow][sourceCol]->setBlock(tiles[targetRow][targetCol]->getBlock());
		tiles[targetRow][targetCol]->setBlock(tmpBlock);
	}
}
