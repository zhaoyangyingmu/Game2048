#pragma once
/**
 * ����ļ��а����˹۲��ߺͱ��۲���
 * ʹ����vector�洢�۲���
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
	 * �ڴ�������ߣ� Observer * ���ⲿ���룬�������ⲿɾ����
	 * ����ʱ�������жϹ۲���vector�Ƿ�Ϊ�ա�
	 * �����Ϊ�գ�˵��Υ�����ڴ�������ߣ�Ӧ�����������Ϣ��
	 */
	~Observable();

	void pushObserver(Observer* observer);
	Observer* popObserver();

	/**
	 * ʹ�ù۲���ģʽ�������۲��߷����仯ʱ��֪ͨ�۲��ߡ�
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
	 * ʹ�ù۲���ģʽ�е�push��pull����
	 * arg��push�е����ݣ��ɱ��۲��߷���һЩ���ݡ�
	 * ���ǣ�����������У���û��ʹ�õ���
	 * o����pull�Ķ���������ȡ����
	 */
	virtual void update(const Observable& o, void * arg) = 0;
};
// for model

