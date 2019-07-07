#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H
enum class Direction;
class Controller; // Controller 指针可用，方法可用。
class Player;
class ConsoleView;
#include "AI.h"
#include "Board.h"
#include "ObserverPattern.h"
#include "Tile.h"
#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;
class ConsoleView : public Observer //实现Observer
{
public:
	ConsoleView() {}
	~ConsoleView();
	void update(const Observable& o, void * arg); // 实现抽象的update方法
	void listen(Controller* controller); // 监听用户输入
};

/**
 * 为了像java那样实现parseDirection的方法，
 * 只好将这个方法放到Controller里面了。
 * 更好的做法应当是放在一个namespace里面
 */
enum class Direction { LEFT, RIGHT, UP, DOWN };


class Controller
{
	// 用于记录时间
	time_t begin;
	time_t end;

	// consoleView是View
	// board是model
	ConsoleView consoleView;
	Board* board;

	// Controller用单例模式实现
	static Controller* controller;
	Controller();
	~Controller();

	// 这里的设计是不对的，应该将显示有关的方法放在View里面才对
	// 或者这里的方法内部向指定的View发消息，让View显示信息。
	// 这样controller可以控制信息的展示。
	void outputCredit(const string&name, const int credit, int timeCost);// 此方法输出分数
	void showRank(); // 此方法输出排名
	void restart(); // 此方法用于重新开始
	void processWin(); // 此方法用于处理获胜的情况
	void processLose(); // 此方法用于处理失败的情况
	void help(); // 此方法用于输出帮助信息
public:
	static Controller& getInstance(); // 获取单例
	static void destroy(); // 销毁单例
	void start(); // 开始游戏
	void executeCommand(const string& command); // 执行命令
};

class Player {
	string name;
public:
	Player() {}
	~Player() {}
	void setName(const string& nm) {
		name = nm;
	}
	const string getName() const {
		return name;
	}
};

#endif // !CONTROLLER_H

