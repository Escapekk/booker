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

// ������ʽ��֤�����ʽ�Ƿ���ȷ
int Booker::isValidFormat(const string& input) {
	regex bookPattern(R"(^([A-Za-z0-9]+)\s(\d{4}-\d{2}-\d{2})\s(\d{2}:00~\d{2}:00)\s([A-D])$)");
	regex cancelPattern(R"(^([A-Za-z0-9]+)\s(\d{4}-\d{2}-\d{2})\s(\d{2}:00~\d{2}:00)\s([A-D])\s([C])$)");

	if (regex_match(input, bookPattern)) {  // Ԥ��
		return 1;
	}
	else if (regex_match(input, cancelPattern)) { // ȡ��Ԥ��
		return 2;
	}
	else { // ��ʽ����
		return 0;
	}
}


// ����Ԥ����¼����
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
// ����ȡ��Ԥ����¼����
void Booker::cancelBooking(Order& order) {
	LinkedList* orderList = placeMap[order.getPlace()];
	if (orderList->updateNode(order)) {
		cout << "Success: the booking is accepted!" << endl;
	}
	else {
		cout << "Error: the booking being cancelled does not exist!" << endl;
	}
}
// ��������
void Booker::dealInput(const string& input) {
	// ��ʽΪ{�û�ID} {Ԥ������ yyyy-MM-dd} {Ԥ��ʱ��� HH:mm~HH:mm} {����} {ȡ�����}
	// ��U123 2016-06-02 20:00~22:00 A C
	int flag = isValidFormat(input);
	if (flag == 0) { //�����ʽ����
		cout << "Error: the booking is invalid!" << endl;
		return;
	}
	Order order;
	order.parseInput(input);
	if (!order.checkTimeValid()) { // ԤԼʱ���������
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

// ���������ϸ
void Booker::print() {
	cout << "�������" << endl;
	cout << "---" << endl;

	double priceSum = 0;
	for (auto it = placeMap.begin(); it != placeMap.end(); ++it) {
		cout << "���أ�" << it->first << endl;
		double price = it->second->printNode();
		priceSum += price;
		cout << "С�ƣ�" << price << "Ԫ" << endl << endl;
	}

	cout << "---" << endl;
	cout << "�ܼƣ�" << priceSum << "Ԫ" << endl;
}