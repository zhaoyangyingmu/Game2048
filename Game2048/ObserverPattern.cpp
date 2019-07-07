#include "stdafx.h"
#include "ObserverPattern.h"

Observable::Observable()
{
	changed = false;
}

/**
* observer list 需要自行清空。
* 为了调试方便，并不直接退出。
*/
Observable::~Observable()
{
	if (obs.size() != 0) {
		cout << "Error: Observer list not empty. " << endl;
	}
}

/**
* 不判断Observer是否重复
*/
void Observable::pushObserver(Observer* observer)
{
	obs.push_back(observer);
}

/**
* 抛出指针后， 自行删除。这样做可以避免内存泄露。
* 假如没有了，就返回NULL。
*/
Observer* Observable::popObserver() {
	if (obs.size() == 0) {
		return NULL;
	}
	Observer* observer = (*obs.rbegin());
	obs.pop_back();
	return observer;
}

void Observable::notifyObservers() {
	if (changed) {
		for (auto ir = obs.begin(); ir != obs.end(); ir++) {
			(*ir)->update(*this, NULL);
		}
	}
	clearChanged();
}