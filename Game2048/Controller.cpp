#include "stdafx.h"
#include "Controller.h"

Controller* Controller::controller = NULL;

Controller& Controller::getInstance() {
	if (controller == NULL) {
		controller = new Controller();
	}
	return *controller;
}

void Controller::destroy() {
	delete controller;
	controller = NULL;
}

Controller::Controller()
{
	// consoleView 已经初始化好了，可以调用update和listen方法
	// board 也已经初始化好了，直接就有了两个随机的2
	// board add observer.
	board = new Board();
	board->pushObserver(&consoleView);

}

Controller::~Controller()
{
	board->popObserver();
	delete board;
}

void Controller::start() {
	cout << "Welcome to game 2048" << endl;
	cout << "Enter your name: ";
	string name;
	while (!getline(cin, name)) {
	}
	help();
	board->getPlayer()->setName(name);
	consoleView.update(*board, NULL);
	begin = time(0);
	consoleView.listen(this);
}

void Controller::restart() {
	// 输入r/R之后，进行调整并返回
	// 输入q/Q之后，退出游戏
	cout << "Enter r to restart the game or q to quit. " << endl;
	char command;
	while (true) {
		if (_kbhit()) {
			command = _getch();
			if (command == 'r' || command == 'R') {
				begin = time(0);
				cout << "Restart Game!" << endl;
				cout << "Enter your name: ";
				string name;
				while (!getline(cin, name)) {
				}
				board->getPlayer()->setName(name);
				board->restart();

				board->setChanged();
				board->notifyObservers();
				return;
			}
			if (command == 'q' || command == 'Q') {
				exit(0);
			}
			cout << "Enter r to restart the game or q to quit. " << endl;
		}
		else {
			// 这里会有50ms的延时，但实际上所有的命令还是保留的。
			// 为了避免执行太快消耗资源。
			Sleep(50);
		}

	}
}

void Controller::executeCommand(const string& command) {
	bool slideResult = false;
	bool moveCommand = false;
	if (command == "w" || command == "W") { // up
		slideResult = board->playerAct(Direction::UP);
		moveCommand = true;
	}
	if (command == "s" || command == "S") { // Down
		slideResult = board->playerAct(Direction::DOWN);
		moveCommand = true;
	}
	if (command == "a" || command == "A") { // left
		slideResult = board->playerAct(Direction::LEFT);
		moveCommand = true;
	}
	if (command == "d" || command == "D") { // right
		slideResult = board->playerAct(Direction::RIGHT);
		moveCommand = true;
	}
	if (slideResult) { //动了
		board->computerAct();
		board->setChanged();
		board->notifyObservers();
		if (board->checkWin()) {
			processWin();
			restart();
			return;
		}
		if (board->checkLose()) {
			processLose();
			restart();
			return;
		}
	}
	if (moveCommand && !slideResult) { // 动不了
		cout << "Sorry, you can't move in this direction. " << endl;
		return;
	}
	if (command == "x" || command == "X") {
		board->destroyMinBlock();
		board->setChanged();
		board->notifyObservers();
	}
	if (command == "z" || command == "Z") {
		board->randomizeBlocks();
		board->setChanged();
		board->notifyObservers();
		if (board->getDestroyLimit() == 0 && board->getRandomLimit() == 0) {
			if (board->checkLose()) {
				processLose();
				restart();
				return;
			}
		}
	}
	if (command == "r" || command == "R") {
		begin = time(0);
		cout << "Restart Game!" << endl;
		cout << "Enter your name: ";
		string name;
		while (!getline(cin, name)) {
		}
		board->getPlayer()->setName(name);
		board->restart();
		board->setChanged();
		board->notifyObservers();
	}
	if (command == "overtime") {
		board->setBlockByRandom();
		board->setChanged();
		board->notifyObservers();
	}
	if (command == "q" || command == "Q") { // quit
		exit(0);
	}
	if (command == "p" || command == "P")
	{
		cout << "p/P is the cheat key." << endl;
		board->cheat();
		board->setChanged();
		board->notifyObservers();
	}
	if (command == "o" || command == "O")
	{
		cout << "o/O is the dead key." << endl;
		board->setDead();
		board->setChanged();
		board->notifyObservers();
	}
	if (command == "l" || command == "L")
	{
		cout << "l/L to show the rank list." << endl;
		showRank();
	}
	if (command == "c" || command == "C") {
		cout << "AI PLAYING" << endl;
		AI ai;
		while (true) {
			Direction dir = ai.findBest(*board);
			bool result = board->playerAct(dir);
			board->computerAct();
			board->setChanged();
			board->notifyObservers();
			if ((board->checkWin())) {
				break;
			}
			if (!result) {
				if ((board->checkLose())) {
					break;
				}
				if (board->getDestroyLimit() != 0) {
					board->destroyMinBlock();
				}
				if (board->getRandomLimit() != 0) {
					board->randomizeBlocks();
				}
			}
		}
		if (board->checkWin()) {
			cout << "Win" << endl;
			processWin();
			restart();
			return;
		}
		if (board->checkLose()) {
			cout << "Lose" << endl;
			processLose();
			restart();
			return;
		}
	}
}

void Controller::outputCredit(const string&name, const int credit, int timeCost)
{
	ofstream out;
	out.open("credit.txt", ios::app | ios::out);
	out << name << endl;
	out << credit << endl;
	out << timeCost << endl;
	out.close();
}

void Controller::processWin() {
	cout << "Congratulations! You have won the game! " << endl;
	end = time(0);
	int timeCost = (int)(end - begin);
	int seconds = timeCost % 60;
	int minutes = timeCost / 60;
	int hours = minutes / 60;
	minutes = minutes % 60;
	cout << "name : " << board->getPlayer()->getName() << " time cost: ";
	if (hours != 0) {
		cout << hours << " hours ";
	}
	if (minutes != 0) {
		cout << minutes << " minutes ";
	}
	cout << seconds << " seconds" << endl;
	outputCredit(board->getPlayer()->getName(), board->getCredit(), timeCost);
}

void Controller::processLose() {
	cout << "Sorry! You lose the game! " << endl;
	end = time(0);
	int timeCost = (int)(end - begin);
	int seconds = timeCost % 60;
	int minutes = timeCost / 60;
	int hours = minutes / 60;
	minutes = minutes % 60;
	cout << "name : " << board->getPlayer()->getName() << " time cost: ";
	if (hours != 0) {
		cout << hours << " hours ";
	}
	if (minutes != 0) {
		cout << minutes << " minutes ";
	}
	cout << seconds << " seconds" << endl;
}

void Controller::showRank()
{
	ifstream in("credit.txt");
	cout << endl;
	cout << "------------------------Rank------------------------" << endl;
	string line;
	while (getline(in, line)) {
		cout << "name: " << line;
		getline(in, line);
		cout << " credit: " << line;
		getline(in, line);
		int seconds = atoi(line.c_str());
		int minutes = seconds / 60;
		seconds %= 60;
		int hours = minutes / 60;
		minutes %= 60;
		cout << " time cost: ";
		if (hours != 0) {
			cout << hours << " hours ";
		}
		if (minutes != 0) {
			cout << minutes << " minutes ";
		}
		cout << seconds << " seconds" << endl;
	}
	in.close();
	cout << "------------------------End------------------------" << endl;
}

void Controller::help() {
	cout << "HELP MESSAGE" << endl;
	cout << "[a] : left" << endl;
	cout << "[d] : right" << endl;
	cout << "[s] : down" << endl;
	cout << "[q] : quit" << endl;
	cout << "[z] : random" << endl;
	cout << "[x] : destroy" << endl;
	cout << "[p] : cheat" << endl;
	cout << "[o] : dead" << endl;
	cout << "[c] : auto" << endl;
	cout << "[l] : show rank" << endl;
	cout << "[r] : restart" << endl;
	cout << endl;

}

ConsoleView::~ConsoleView()
{
}


void ConsoleView::update(const Observable& o, void * arg)
{
	Board& board = (Board&)o;
	cout << "GAME 2048" << endl;
	cout << "Your name: " << (board.getPlayer())->getName() << "     Your credit: " << board.getCredit() << endl;
	cout << "RandomTool: " << board.getRandomLimit() << "      DestroyTool: " << board.getDestroyLimit() << endl;
	cout << "Time limit: 2 minutes" << endl;
	cout << "-----------------------------" << endl;
	for (int row = 0; row < board.ROW_NUMBER; row++) {
		cout << "|";
		for (int col = 0; col < board.COL_NUMBER; col++) {
			Block* block = board.tiles[row][col]->getBlock();
			if (block != NULL) {
				int number = block->getNumber();
				cout << setw(6) << number << "|";
			}
			else {
				cout << "      " << "|";
			}
		}
		cout << endl;
		cout << "-----------------------------" << endl;
	}
	cout << endl;
}

void ConsoleView::listen(Controller* controller) {
	const int TIME_LIMIT = 120;
	char command;
	time_t begin = time(0);
	while (true) {
		if (_kbhit()) {
			command = _getch();
			string line = "a";
			line[0] = command;
			controller->executeCommand(line);
			begin = time(0);
		}
		else if (time(0) - begin >= TIME_LIMIT) {
			cout << "You are out of time. " << endl;
			string line = "overtime";
			controller->executeCommand(line);
			begin = time(0);
		}
		else {
			// 这里会有50ms的延时，但实际上所有的命令还是保留的。
			Sleep(50);
		}

	}
}
