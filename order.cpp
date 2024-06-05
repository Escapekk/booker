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
// ��֤����ʱ������Ƿ���Ч
bool Order::checkTimeValid() {
	if (date.startTime < 9 || date.startTime > 22) return false;
	if (date.endTime < 9 || date.endTime > 22) return false;
	if (date.startTime >= date.endTime) return false;
	return true;
}

// ���㶩���۸�
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

// ���������ö�����Ϣ
void Order::parseInput(const string& input) {
	istringstream iss(input);
	string userIDStr, dateStr, timeStr, placeStr;
	iss >> userIDStr >> dateStr >> timeStr >> placeStr;
	userID = userIDStr;
	date.parseDatetime(dateStr, timeStr);
	place = placeStr;
}

// ȡ������������ΥԼ��
void Order::updateOrder() {
	isCancelled = true;
	if (date.weekday >= 1 && date.weekday <= 5) {
		price *= 0.5;
	}
	else {
		price *= 0.25;
	}
}
// ��ӡ������Ϣ
double Order::printOrder() {
	std::cout << setw(4) << setfill('0') << date.year
		<< "-" << setw(2) << setfill('0') << date.month
		<< "-" << setw(2) << setfill('0') << date.day
		<< " " << setw(2) << setfill('0') << date.startTime
		<< ":00~" << setw(2) << setfill('0') << date.endTime
		<< ":00 " << (isCancelled ? "ΥԼ�� " : "") << price << "Ԫ" << endl;
	return price;
}
// �ж���������ʱ���Ƿ��ͻ
bool Order::isConflict(const Order& order) {
	return date.haveIntersection(order.date);
}
// �ж�����������ʼʱ���С
bool Order::isLater(const Order& order) {
	return date.isLater(order.date);
}
// �ж϶����Ƿ�ȡ��
bool Order::checkCancelled() {
	return isCancelled;
}