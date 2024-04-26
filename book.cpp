#include<iostream>
#include<string>
#include<sstream>
#include<regex>
#include<map>
#include<iomanip>
using namespace std;

// 时间类
struct Datetime {
	int year;  //年
	int month;  //月
	int day;  //日
	int weekday;  //星期几
	int startTime;  //开始时间
	int endTime;  //结束时间

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

	// 判断两个时间区间是否有交集
	bool haveIntersection(const Datetime& datetime) {
		if (year == datetime.year && month == datetime.month && day == datetime.day) {
			return startTime < datetime.endTime&& endTime > datetime.startTime;
		}
		return false;
	}

	// 判断当前时间是否晚于datetime
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

// 订单类
struct Order {
	string userID; // 用户ID
	Datetime date; // 订单时间
	string place; //场地
	double price;  //订单收入
	bool isCancelled = false; //是否已取消

	bool operator==(const Order& order) {
		return userID == order.userID && date == order.date && place == order.place && isCancelled == order.isCancelled;
	}
};
// 链表节点：用于存储订单记录
struct Node {
	Order* order; // 指向订单记录
	Node* next;  // 指向下一个节点

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

// 链表类：用以按时间排序预定记录
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
	// 判断新增订单是否和原订单冲突
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
	// 订单链表新增订单
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
	// 取消订单，修改订单状态
	bool updateNode(const Order& order) {
		Node* cur = head->next;
		while (cur != nullptr) {
			if (*cur->order == order) {
				cur->order->isCancelled = true;
				if (order.date.weekday >= 1 && order.date.weekday <= 5) {
					cur->order->price *= 0.5;  //计算违约金
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
	// 输出链表节点信息
	double printNode() {
		double price = 0;
		for (Node* cur = head->next; cur != nullptr; cur = cur->next) {
			price += cur->order->price;
			cout << cur->order->date.year
				<< "-" << setw(2) << setfill('0') << cur->order->date.month
				<< "-" << setw(2) << setfill('0') << cur->order->date.day
				<< " " << setw(2) << setfill('0') << cur->order->date.startTime
				<< ":00~" << setw(2) << setfill('0') << cur->order->date.endTime
				<< ":00 " << (cur->order->isCancelled ? "违约金 " : "") << cur->order->price << "元" << endl;
		}
		return price;
	}

private:
	Node* head;  // 链表头节点
};

// 根据场地映射链表
map<string, LinkedList*> placeMap;

// 正则表达式验证输入格式是否正确
int isValidFormat(const string& input) {
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
// 根据日期计算星期几
int getWeekdayByDate(int y, int m, int d) {
	if (m <= 2) {
		m += 12;
		y -= 1;
	}
	return (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7 + 1;
}
// 解析字符串获得日期时间
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
// 解析输入获得订单信息
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
// 验证订单时间参数是否有效
bool checkTimeValid(const Order& order) {
	int startTime = order.date.startTime;
	int endTime = order.date.endTime;
	if (startTime < 9 || startTime > 22) return false;
	if (endTime < 9 || endTime > 22) return false;
	if (startTime >= endTime) return false;
	return true;
}
// 计算订单价格
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
// 处理预定记录操作
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
// 处理取消预定记录操作
void cancelBooking(LinkedList* orderList, Order& order) {
	if (orderList->updateNode(order)) {
		cout << "Success: the booking is accepted!" << endl;
	}
	else {
		cout << "Error: the booking being cancelled does not exist!" << endl;
	}
}
// 处理输入
void dealInput(const string& input) {
	// 格式为{用户ID} {预订日期 yyyy-MM-dd} {预订时间段 HH:mm~HH:mm} {场地} {取消标记}
	// 如U123 2016-06-02 20:00~22:00 A C
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

// 输出
void print() {
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
// 初始化
void init() {
	placeMap["A"] = new LinkedList();
	placeMap["B"] = new LinkedList();
	placeMap["C"] = new LinkedList();
	placeMap["D"] = new LinkedList();
}
// 释放内存
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

> 收入汇总
> ---
> 场地:A
> 2017-08-01 19:00~22:00 200元
> 小计：200元
>
> 场地:B
> 2017-08-02 13:00~17:00 200元
> 小计：200元
>
> 场地:C
> 2017-08-03 15:00~16:00 50元
> 小计：50元
>
> 场地:D
> 2017-08-05 09:00~11:00 80元
> 小计：80元
> ---
> 总计：530元

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

> 收入汇总
> ---
> 场地:A
> 2017-08-01 18:00~20:00 160元
> 2017-08-01 19:00~22:00 违约金 100元
> 小计：260元
>
> 场地:B
> 2017-08-02 13:00~17:00 200元
> 小计：200元
>
> 场地:C
> 小计：0元
>
> 场地:D
> 小计：0元
> ---
> 总计：460元
*/