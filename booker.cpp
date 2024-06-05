#include "booker.h"
#include <string>
#include<regex>
using namespace std;

Booker::Booker() {
	placeMap["A"] = new LinkedList();
	placeMap["B"] = new LinkedList();
	placeMap["C"] = new LinkedList();
	placeMap["D"] = new LinkedList();
}
Booker::~Booker() {
	for (auto it = placeMap.begin(); it != placeMap.end(); ++it) {
		delete it->second;
	}
	placeMap.clear();
}

// 正则表达式验证输入格式是否正确
int Booker::isValidFormat(const string& input) {
	regex bookPattern(R"(^([A-Za-z0-9]+)\s(\d{4}-\d{2}-\d{2})\s(\d{2}:00~\d{2}:00)\s([A-D])$)");
	regex cancelPattern(R"(^([A-Za-z0-9]+)\s(\d{4}-\d{2}-\d{2})\s(\d{2}:00~\d{2}:00)\s([A-D])\s([C])$)");

	if (regex_match(input, bookPattern)) {  // 预定
		return 1;
	}
	else if (regex_match(input, cancelPattern)) { // 取消预定
		return 2;
	}
	else { // 格式错误
		return 0;
	}
}


// 处理预定记录操作
void Booker::booking(Order& order) {
	LinkedList* orderList = placeMap[order.getPlace()];
	if (orderList->isConflict(order)) {
		cout << "Error: the booking conflicts with existing bookings!" << endl;
		return;
	}
	order.getPrice();
	orderList->addNode(order);
	cout << "Success: the booking is accepted!" << endl;
}
// 处理取消预定记录操作
void Booker::cancelBooking(Order& order) {
	LinkedList* orderList = placeMap[order.getPlace()];
	if (orderList->updateNode(order)) {
		cout << "Success: the booking is accepted!" << endl;
	}
	else {
		cout << "Error: the booking being cancelled does not exist!" << endl;
	}
}
// 处理输入
void Booker::dealInput(const string& input) {
	// 格式为{用户ID} {预订日期 yyyy-MM-dd} {预订时间段 HH:mm~HH:mm} {场地} {取消标记}
	// 如U123 2016-06-02 20:00~22:00 A C
	int flag = isValidFormat(input);
	if (flag == 0) { //输入格式错误
		cout << "Error: the booking is invalid!" << endl;
		return;
	}
	Order order;
	order.parseInput(input);
	if (!order.checkTimeValid()) { // 预约时间参数错误
		cout << "Error: the booking is invalid!" << endl;
		return;
	}
	if (flag == 1) {
		booking(order);
	}
	else {
		cancelBooking(order);
	}
}

// 输出订单明细
void Booker::print() {
	cout << "收入汇总" << endl;
	cout << "---" << endl;

	double priceSum = 0;
	for (auto it = placeMap.begin(); it != placeMap.end(); ++it) {
		cout << "场地：" << it->first << endl;
		double price = it->second->printNode();
		priceSum += price;
		cout << "小计：" << price << "元" << endl << endl;
	}

	cout << "---" << endl;
	cout << "总计：" << priceSum << "元" << endl;
}