#include "stdafx.h"
#include "ObserverPattern.h"

Observable::Observable()
{
	changed = false;
}

/**
* observer list ��Ҫ������ա�
* Ϊ�˵��Է��㣬����ֱ���˳���
*/
Observable::~Observable()
{
	if (obs.size() != 0) {
		cout << "Error: Observer list not empty. " << endl;
	}
}

/**
* ���ж�Observer�Ƿ��ظ�
*/
void Observable::pushObserver(Observer* observer)
{
	obs.push_back(observer);
}

/**
* �׳�ָ��� ����ɾ�������������Ա����ڴ�й¶��
* ����û���ˣ��ͷ���NULL��
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