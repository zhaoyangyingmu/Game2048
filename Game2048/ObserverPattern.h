#pragma once
/**
 * 这个文件中包含了观察者和被观察者
 * 使用了vector存储观察者
 */

#include <vector>
#include <iostream>
using namespace std;

class Observer;

class Observable
{
	bool changed;
	vector<Observer *> obs;
public:
	Observable();
	/**
	 * 内存管理政策： Observer * 从外部传入，并且由外部删除。
	 * 析构时，必须判断观察者vector是否为空。
	 * 如果不为空，说明违反了内存管理政策，应该输出报错信息。
	 */
	~Observable();

	void pushObserver(Observer* observer);
	Observer* popObserver();

	/**
	 * 使用观察者模式，当被观察者发生变化时，通知观察者。
	 */
	void notifyObservers();
	void clearChanged() {
		changed = false;
	}
	void setChanged() {
		changed = true;
	}
	bool hasChanged() {
		return changed;
	}
	int countObservers() {
		return obs.size();
	}
};

class Observer
{
public:
	/**
	 * 使用观察者模式中的push和pull机制
	 * arg是push中的数据，由被观察者发送一些数据。
	 * 但是，在这个程序中，并没有使用到。
	 * o则是pull的对象，用于拉取数据
	 */
	virtual void update(const Observable& o, void * arg) = 0;
};
// for model

