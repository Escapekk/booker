#pragma once

#include "order.h"
#include "linkedlist.h"

#include<string>
#include<regex>
#include<iostream>
#include<map>
using namespace std;

class Booker {
public:
	Booker();
	~Booker();
	// ��������
	void dealInput(const string& input);

	// ���������ϸ
	void print();

private:
	// ���ݳ���ӳ������
	map<string, LinkedList*> placeMap;

	// ������ʽ��֤�����ʽ�Ƿ���ȷ
	int isValidFormat(const string& input);

	// ����Ԥ����¼����
	void booking(Order& order);

	// ����ȡ��Ԥ����¼����
	void cancelBooking(Order& order);
};