#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H
enum class Direction;
class Controller; // Controller ָ����ã��������á�
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
class ConsoleView : public Observer //ʵ��Observer
{
public:
	ConsoleView() {}
	~ConsoleView();
	void update(const Observable& o, void * arg); // ʵ�ֳ����update����
	void listen(Controller* controller); // �����û�����
};

/**
 * Ϊ����java����ʵ��parseDirection�ķ�����
 * ֻ�ý���������ŵ�Controller�����ˡ�
 * ���õ�����Ӧ���Ƿ���һ��namespace����
 */
enum class Direction { LEFT, RIGHT, UP, DOWN };


class Controller
{
	// ���ڼ�¼ʱ��
	time_t begin;
	time_t end;

	// consoleView��View
	// board��model
	ConsoleView consoleView;
	Board* board;

	// Controller�õ���ģʽʵ��
	static Controller* controller;
	Controller();
	~Controller();

	// ���������ǲ��Եģ�Ӧ�ý���ʾ�йصķ�������View����Ŷ�
	// ��������ķ����ڲ���ָ����View����Ϣ����View��ʾ��Ϣ��
	// ����controller���Կ�����Ϣ��չʾ��
	void outputCredit(const string&name, const int credit, int timeCost);// �˷����������
	void showRank(); // �˷����������
	void restart(); // �˷����������¿�ʼ
	void processWin(); // �˷������ڴ����ʤ�����
	void processLose(); // �˷������ڴ���ʧ�ܵ����
	void help(); // �˷����������������Ϣ
public:
	static Controller& getInstance(); // ��ȡ����
	static void destroy(); // ���ٵ���
	void start(); // ��ʼ��Ϸ
	void executeCommand(const string& command); // ִ������
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

