#include<iostream>
#include<string>
#include<sstream>
#include<regex>
#include<map>
#include<iomanip>
using namespace std;

// ʱ����
struct Datetime {
	int year;  //��
	int month;  //��
	int day;  //��
	int weekday;  //���ڼ�
	int startTime;  //��ʼʱ��
	int endTime;  //����ʱ��

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

	// �ж�����ʱ�������Ƿ��н���
	bool haveIntersection(const Datetime& datetime) {
		if (year == datetime.year && month == datetime.month && day == datetime.day) {
			return startTime < datetime.endTime&& endTime > datetime.startTime;
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
};

// ������
struct Order {
	string userID; // �û�ID
	Datetime date; // ����ʱ��
	string place; //����
	double price;  //��������
	bool isCancelled = false; //�Ƿ���ȡ��

	bool operator==(const Order& order) {
		return userID == order.userID && date == order.date && place == order.place && isCancelled == order.isCancelled;
	}
};
// ����ڵ㣺���ڴ洢������¼
struct Node {
	Order* order; // ָ�򶩵���¼
	Node* next;  // ָ����һ���ڵ�

	Node() :order(nullptr), next(nullptr) {}
	Node(const Order& _order) :order(new Order()), next(nullptr) {
		order->userID = _order.userID;
		order->date = _order.date;
		order->place = _order.place;
		order->price = _order.price;
	}
	~Node() {
		delete order;
	}
};

// �����ࣺ���԰�ʱ������Ԥ����¼
class LinkedList {
public:
	LinkedList() :head(new Node()) {}
	~LinkedList() {
		Node* cur = head;
		while (cur != nullptr) {
			Node* next = cur->next;
			delete cur;
			cur = next;
		}
		head = nullptr;
	}
	// �ж����������Ƿ��ԭ������ͻ
	bool isConflict(const Order& order) {
		Node* cur = head->next;
		while (cur != nullptr) {
			if (!cur->order->isCancelled && cur->order->date.haveIntersection(order.date)) {
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
			if (next == nullptr || next->order->date.isLater(node->order->date)) {
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
				cur->order->isCancelled = true;
				if (order.date.weekday >= 1 && order.date.weekday <= 5) {
					cur->order->price *= 0.5;  //����ΥԼ��
				}
				else {
					cur->order->price *= 0.25;
				}
				return true;
			}
			cur = cur->next;
		}
		return false;
	}
	// �������ڵ���Ϣ
	double printNode() {
		double price = 0;
		for (Node* cur = head->next; cur != nullptr; cur = cur->next) {
			price += cur->order->price;
			cout << cur->order->date.year
				<< "-" << setw(2) << setfill('0') << cur->order->date.month
				<< "-" << setw(2) << setfill('0') << cur->order->date.day
				<< " " << setw(2) << setfill('0') << cur->order->date.startTime
				<< ":00~" << setw(2) << setfill('0') << cur->order->date.endTime
				<< ":00 " << (cur->order->isCancelled ? "ΥԼ�� " : "") << cur->order->price << "Ԫ" << endl;
		}
		return price;
	}

private:
	Node* head;  // ����ͷ�ڵ�
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
// �������ڼ������ڼ�
int getWeekdayByDate(int y, int m, int d) {
	if (m <= 2) {
		m += 12;
		y -= 1;
	}
	return (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7 + 1;
}
// �����ַ����������ʱ��
void parseDatetime(const string& dateStr, const string& timeStr, Datetime& date) {
	// dateStr: 2016-06-02
	// timeStr: 20:00~22:00
	date.year = stoi(dateStr.substr(0, 4));
	date.month = stoi(dateStr.substr(5, 2));
	date.day = stoi(dateStr.substr(8));
	date.weekday = getWeekdayByDate(date.year, date.month, date.day);

	date.startTime = stoi(timeStr.substr(0, 2));
	date.endTime = stoi(timeStr.substr(6, 2));
}
// ���������ö�����Ϣ
void parseInput(const string& input, Order& order) {
	istringstream iss(input);
	string userID, date, time, place;
	iss >> userID >> date >> time >> place;
	order.userID = userID;
	parseDatetime(date, time, order.date);
	order.place = place;

	/*
	cout << order.userID << " " << order.date.year << " "
		<< order.date.month << " " << order.date.day << " " << order.date.weekday
		<< " " << order.date.startTime << " " << order.date.endTime
		<< " " << place << endl;
	*/
}
// ��֤����ʱ������Ƿ���Ч
bool checkTimeValid(const Order& order) {
	int startTime = order.date.startTime;
	int endTime = order.date.endTime;
	if (startTime < 9 || startTime > 22) return false;
	if (endTime < 9 || endTime > 22) return false;
	if (startTime >= endTime) return false;
	return true;
}
// ���㶩���۸�
void getPrice(Order& order) {
	int weekday = order.date.weekday;
	int startTime = order.date.startTime;
	int endTime = order.date.endTime;
	double price = 0;
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
	order.price = price;
}
// ����Ԥ����¼����
void booking(LinkedList* orderList, Order& order) {
	if (orderList->isConflict(order)) {
		cout << "Error: the booking conflicts with existing bookings!" << endl;
		return;
	}
	getPrice(order);
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
	if (flag == 0) {
		cout << "Error: the booking is invalid!" << endl;
		return;
	}
	Order order;
	parseInput(input, order);
	if (!checkTimeValid(order)) {
		cout << "Error: the booking is invalid!" << endl;
		return;
	}
	if (flag == 1) {
		booking(placeMap[order.place], order);
	}
	else {
		cancelBooking(placeMap[order.place], order);
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