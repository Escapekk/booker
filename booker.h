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
	// 处理输入
	void dealInput(const string& input);

	// 输出订单明细
	void print();

private:
	// 根据场地映射链表
	map<string, LinkedList*> placeMap;

	// 正则表达式验证输入格式是否正确
	int isValidFormat(const string& input);

	// 处理预定记录操作
	void booking(Order& order);

	// 处理取消预定记录操作
	void cancelBooking(Order& order);
};