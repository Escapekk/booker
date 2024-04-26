#include<iostream>
#include<string>
#include<sstream>
#include<regex>
#include<map>
#include<iomanip>
using namespace std;

// ʱ����
class Datetime {
	friend class Order;

public:
	bool operator==(const Datetime& datetime) {
		return year == datetime.year && month == datetime.month && day == datetime.day &&
			startTime == datetime.startTime && endTime == datetime.endTime;
	}
	Datetime& operator=(const Datetime& datetime) {
		year = datetime.year;
		month = datetime.month;
		day = datetime.day;
		weekday = datetime.weekday;
		startTime = datetime.startTime;
		endTime = datetime.endTime;
		return *this;
	}

	// �����ַ����������ʱ��
	void parseDatetime(const string& dateStr, const string& timeStr) {
		// dateStr: 2016-06-02
		// timeStr: 20:00~22:00
		year = stoi(dateStr.substr(0, 4));
		month = stoi(dateStr.substr(5, 2));
		day = stoi(dateStr.substr(8));
		getWeekdayByDate();

		startTime = stoi(timeStr.substr(0, 2));
		endTime = stoi(timeStr.substr(6, 2));
	}

	// �ж�����ʱ�������Ƿ��н���
	bool haveIntersection(const Datetime& datetime) {
		if (year == datetime.year && month == datetime.month && day == datetime.day) {
			return startTime < datetime.endTime && endTime > datetime.startTime;
		}
		return false;
	}

	// �жϵ�ǰʱ���Ƿ�����datetime
	bool isLater(const Datetime& datetime) {
		if (year > datetime.year) {
			return true;
		}
		else if (year < datetime.year) {
			return false;
		}
		else {
			if (month > datetime.month) {
				return true;
			}
			else if (month < datetime.month) {
				return false;
			}
			else {
				if (day > datetime.day) {
					return true;
				}
				else if (day < datetime.day) {
					return false;
				}
				else {
					return startTime > datetime.startTime;
				}
			}
		}
	}
private:
	int year;  //��
	int month;  //��
	int day;  //��
	int weekday;  //���ڼ�
	int startTime;  //��ʼʱ��
	int endTime;  //����ʱ��

	// �������ڼ������ڼ�
	int getWeekdayByDate() {
		int y = year, m = month, d = day;
		if (m <= 2) {
			m += 12;
			y -= 1;
		}
		weekday = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7 + 1;
		return weekday;
	}
};

// ������
class Order {
public:
	Order() {}
	Order(const Order& order) {
		userID = order.userID;
		date = order.date;
		place = order.place;
		price = order.price;
		isCancelled = order.isCancelled;
	}
	bool operator==(const Order& order) {
		return userID == order.userID && date == order.date && place == order.place && isCancelled == order.isCancelled;
	}
	string& getPlace() {
		return place;
	}
	// ��֤����ʱ������Ƿ���Ч
	bool checkTimeValid() {
		if (date.startTime < 9 || date.startTime > 22) return false;
		if (date.endTime < 9 || date.endTime > 22) return false;
		if (date.startTime >= date.endTime) return false;
		return true;
	}

	// ���㶩���۸�
	void getPrice() {
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
	void parseInput(const string& input) {
		istringstream iss(input);
		string userIDStr, dateStr, timeStr, placeStr;
		iss >> userIDStr >> dateStr >> timeStr >> placeStr;
		userID = userIDStr;
		date.parseDatetime(dateStr, timeStr);
		place = placeStr;
	}

	// ȡ������������ΥԼ��
	void updateOrder() {
		isCancelled = true;
		if (date.weekday >= 1 && date.weekday <= 5) {
			price *= 0.5;
		}
		else {
			price *= 0.25;
		}
	}
	// ��ӡ������Ϣ
	double printOrder() {
		cout << setw(4) << setfill('0') << date.year
			<< "-" << setw(2) << setfill('0') << date.month
			<< "-" << setw(2) << setfill('0') << date.day
			<< " " << setw(2) << setfill('0') << date.startTime
			<< ":00~" << setw(2) << setfill('0') << date.endTime
			<< ":00 " << (isCancelled ? "ΥԼ�� " : "") << price << "Ԫ" << endl;
		return price;
	}
	// �ж���������ʱ���Ƿ��ͻ
	bool isConflict(const Order& order) {
		return date.haveIntersection(order.date);
	}
	// �ж�����������ʼʱ���С
	bool isLater(const Order& order) {
		return date.isLater(order.date);
	}
	// �ж϶����Ƿ�ȡ��
	bool checkCancelled() {
		return isCancelled;
	}
private:
	string userID; // �û�ID
	Datetime date; // ����ʱ��
	string place; //����
	double price = 0;  //��������
	bool isCancelled = false; //�Ƿ���ȡ��
};
// ����ڵ㣺���ڴ洢������¼
struct Node {
	Order *order; // ָ�򶩵���¼
	Node *next;  // ָ����һ���ڵ�

	Node():order(nullptr), next(nullptr){}
	Node(const Order& _order):order(new Order(_order)), next(nullptr){}
	~Node() {
		delete order;
	}
};

// �����ࣺ���԰�ʱ������Ԥ����¼
class LinkedList {
public:
	LinkedList():head(new Node()){}
	~LinkedList() {
		Node* cur = head;
		while (cur != nullptr) {
			Node* next = cur->next;
			delete cur;
			cur = next;
		}
		head = nullptr;
	}
	// �ж��Ƿ������������
	bool isConflict(const Order &order) {
		Node* cur = head->next;
		while (cur != nullptr) {
			if (!cur->order->checkCancelled() && cur->order->isConflict(order)) {
				return true;
			}
			cur = cur->next;
		}
		return false;
	}
	// ����������������
	void addNode(Node* node) {
		Node* cur = head;
		while (cur != nullptr) {
			Node* next = cur->next;
			if (next == nullptr || next->order->isLater(*node->order)) {
				node->next = next;
				cur->next = node;
				break;
			}
			cur = cur->next;
		}
	}
	// ȡ���������޸Ķ���״̬
	bool updateNode(const Order& order) {
		Node* cur = head->next;
		while (cur != nullptr) {
			if (*cur->order == order) {
				cur->order->updateOrder();
				return true;
			}
			cur = cur->next;
		}
		return false;
	}
	// �������ڵ���Ϣ,����������
	double printNode() {
		double price = 0;
		for (Node* cur = head->next; cur != nullptr; cur = cur->next) {
			price += cur->order->printOrder();
		}
		return price;
	}
	
private:
	Node *head;  // ����ͷ�ڵ�
};

// ���ݳ���ӳ������
map<string, LinkedList*> placeMap;

// ������ʽ��֤�����ʽ�Ƿ���ȷ
int isValidFormat(const string& input) {
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
void booking(LinkedList* orderList, Order& order) {
	if (orderList->isConflict(order)) {
		cout << "Error: the booking conflicts with existing bookings!" << endl;
		return;
	}
	order.getPrice();
	Node* node = new Node(order);
	orderList->addNode(node);
	cout << "Success: the booking is accepted!" << endl;
}
// ����ȡ��Ԥ����¼����
void cancelBooking(LinkedList* orderList, Order& order) {
	if (orderList->updateNode(order)) {
		cout << "Success: the booking is accepted!" << endl;
	}
	else {
		cout << "Error: the booking being cancelled does not exist!" << endl;
	}
}
// ��������
void dealInput(const string& input) {
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
		booking(placeMap[order.getPlace()], order);
	}
	else {
		cancelBooking(placeMap[order.getPlace()], order);
	}
}

// ���
void print() {
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
// ��ʼ��
void init() {
	placeMap["A"] = new LinkedList();
	placeMap["B"] = new LinkedList();
	placeMap["C"] = new LinkedList();
	placeMap["D"] = new LinkedList();
}
// �ͷ��ڴ�
void clean() {
	for (auto it = placeMap.begin(); it != placeMap.end(); ++it) {
		delete it->second;
	}
	placeMap.clear();
}
int main() {
	init();
	string input = "";
	while (getline(cin, input)) {
		if (input == "") {
			print();
			break;
		}
		else {
			dealInput(input);
		}
	}
	clean();
	return 0;
}

/*
case 1:
abcdefghijklmnopqrst1234567890
U001 2016-06-02 22:00~22:00 A
U002 2017-08-01 19:00~22:00 A
U003 2017-08-02 13:00~17:00 B
U004 2017-08-03 15:00~16:00 C
U005 2017-08-05 09:00~11:00 D

abcdefghijklmnopqrst1234567890
> Error: the booking is invalid!
U001 2016-06-02 22:00~22:00 A
> Error: the booking is invalid!
U002 2017-08-01 19:00~22:00 A
> Success: the booking is accepted!
U003 2017-08-02 13:00~17:00 B
> Success: the booking is accepted!
U004 2017-08-03 15:00~16:00 C
> Success: the booking is accepted!
U005 2017-08-05 09:00~11:00 D
> Success: the booking is accepted!

> �������
> ---
> ����:A
> 2017-08-01 19:00~22:00 200Ԫ
> С�ƣ�200Ԫ
>
> ����:B
> 2017-08-02 13:00~17:00 200Ԫ
> С�ƣ�200Ԫ
>
> ����:C
> 2017-08-03 15:00~16:00 50Ԫ
> С�ƣ�50Ԫ
>
> ����:D
> 2017-08-05 09:00~11:00 80Ԫ
> С�ƣ�80Ԫ
> ---
> �ܼƣ�530Ԫ

case 2:
U002 2017-08-01 19:00~22:00 A
U003 2017-08-01 18:00~20:00 A
U002 2017-08-01 19:00~22:00 A C
U002 2017-08-01 19:00~22:00 A C
U003 2017-08-01 18:00~20:00 A
U003 2017-08-02 13:00~17:00 B

U002 2017-08-01 19:00~22:00 A
> Success: the booking is accepted!
U003 2017-08-01 18:00~20:00 A
> Error: the booking conflicts with existing bookings!
U002 2017-08-01 19:00~22:00 A C
> Success: the booking is accepted!
U002 2017-08-01 19:00~22:00 A C
> Error: the booking being cancelled does not exist!
U003 2017-08-01 18:00~20:00 A
> Success: the booking is accepted!
U003 2017-08-02 13:00~17:00 B
> Success: the booking is accepted!

> �������
> ---
> ����:A
> 2017-08-01 18:00~20:00 160Ԫ
> 2017-08-01 19:00~22:00 ΥԼ�� 100Ԫ
> С�ƣ�260Ԫ
>
> ����:B
> 2017-08-02 13:00~17:00 200Ԫ
> С�ƣ�200Ԫ
>
> ����:C
> С�ƣ�0Ԫ
>
> ����:D
> С�ƣ�0Ԫ
> ---
> �ܼƣ�460Ԫ
*/