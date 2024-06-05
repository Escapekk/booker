#include "order.h"
#include<string>
#include<sstream>
#include<iomanip>
#include<iostream>


Order::Order(const Order& order) {
	userID = order.userID;
	date = order.date;
	place = order.place;
	price = order.price;
	isCancelled = order.isCancelled;
}
bool Order::operator==(const Order& order) {
	return userID == order.userID && date == order.date && place == order.place && isCancelled == order.isCancelled;
}
string& Order::getPlace() {
	return place;
}
// 验证订单时间参数是否有效
bool Order::checkTimeValid() {
	if (date.startTime < 9 || date.startTime > 22) return false;
	if (date.endTime < 9 || date.endTime > 22) return false;
	if (date.startTime >= date.endTime) return false;
	return true;
}

// 计算订单价格
void Order::getPrice() {
	int weekday = date.weekday;
	int startTime = date.startTime;
	int endTime = date.endTime;
	if (weekday >= 1 && weekday <= 5) {
		for (int i = startTime; i < endTime; ++i) {
			if (i >= 9 && i < 12) price += 30;
			else if (i >= 12 && i < 18) price += 50;
			else if (i >= 18 && i < 20) price += 80;
			else price += 60;
		}
	}
	else {
		for (int i = startTime; i < endTime; ++i) {
			if (i >= 9 && i < 12) price += 40;
			else if (i >= 12 && i < 18) price += 50;
			else price += 60;
		}
	}
}

// 解析输入获得订单信息
void Order::parseInput(const string& input) {
	istringstream iss(input);
	string userIDStr, dateStr, timeStr, placeStr;
	iss >> userIDStr >> dateStr >> timeStr >> placeStr;
	userID = userIDStr;
	date.parseDatetime(dateStr, timeStr);
	place = placeStr;
}

// 取消订单并计算违约金
void Order::updateOrder() {
	isCancelled = true;
	if (date.weekday >= 1 && date.weekday <= 5) {
		price *= 0.5;
	}
	else {
		price *= 0.25;
	}
}
// 打印订单信息
double Order::printOrder() {
	std::cout << setw(4) << setfill('0') << date.year
		<< "-" << setw(2) << setfill('0') << date.month
		<< "-" << setw(2) << setfill('0') << date.day
		<< " " << setw(2) << setfill('0') << date.startTime
		<< ":00~" << setw(2) << setfill('0') << date.endTime
		<< ":00 " << (isCancelled ? "违约金 " : "") << price << "元" << endl;
	return price;
}
// 判断两个订单时间是否冲突
bool Order::isConflict(const Order& order) {
	return date.haveIntersection(order.date);
}
// 判断两个订单起始时间大小
bool Order::isLater(const Order& order) {
	return date.isLater(order.date);
}
// 判断订单是否取消
bool Order::checkCancelled() {
	return isCancelled;
}