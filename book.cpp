#include<iostream>
#include<string>
#include<sstream>
#include<regex>
#include<map>
#include<iomanip>
using namespace std;

// 时间类
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

	// 解析字符串获得日期时间
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

	// 判断两个时间区间是否有交集
	bool haveIntersection(const Datetime& datetime) {
		if (year == datetime.year && month == datetime.month && day == datetime.day) {
			return startTime < datetime.endTime && endTime > datetime.startTime;
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
private:
	int year;  //年
	int month;  //月
	int day;  //日
	int weekday;  //星期几
	int startTime;  //开始时间
	int endTime;  //结束时间

	// 根据日期计算星期几
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

// 订单类
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
	// 验证订单时间参数是否有效
	bool checkTimeValid() {
		if (date.startTime < 9 || date.startTime > 22) return false;
		if (date.endTime < 9 || date.endTime > 22) return false;
		if (date.startTime >= date.endTime) return false;
		return true;
	}

	// 计算订单价格
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

	// 解析输入获得订单信息
	void parseInput(const string& input) {
		istringstream iss(input);
		string userIDStr, dateStr, timeStr, placeStr;
		iss >> userIDStr >> dateStr >> timeStr >> placeStr;
		userID = userIDStr;
		date.parseDatetime(dateStr, timeStr);
		place = placeStr;
	}

	// 取消订单并计算违约金
	void updateOrder() {
		isCancelled = true;
		if (date.weekday >= 1 && date.weekday <= 5) {
			price *= 0.5;
		}
		else {
			price *= 0.25;
		}
	}
	// 打印订单信息
	double printOrder() {
		cout << setw(4) << setfill('0') << date.year
			<< "-" << setw(2) << setfill('0') << date.month
			<< "-" << setw(2) << setfill('0') << date.day
			<< " " << setw(2) << setfill('0') << date.startTime
			<< ":00~" << setw(2) << setfill('0') << date.endTime
			<< ":00 " << (isCancelled ? "违约金 " : "") << price << "元" << endl;
		return price;
	}
	// 判断两个订单时间是否冲突
	bool isConflict(const Order& order) {
		return date.haveIntersection(order.date);
	}
	// 判断两个订单起始时间大小
	bool isLater(const Order& order) {
		return date.isLater(order.date);
	}
	// 判断订单是否取消
	bool checkCancelled() {
		return isCancelled;
	}
private:
	string userID; // 用户ID
	Datetime date; // 订单时间
	string place; //场地
	double price = 0;  //订单收入
	bool isCancelled = false; //是否已取消
};
// 链表节点：用于存储订单记录
struct Node {
	Order *order; // 指向订单记录
	Node *next;  // 指向下一个节点

	Node():order(nullptr), next(nullptr){}
	Node(const Order& _order):order(new Order(_order)), next(nullptr){}
	~Node() {
		delete order;
	}
};

// 链表类：用以按时间排序预定记录
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
	// 判断是否可以新增订单
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
	// 订单链表新增订单
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
	// 取消订单，修改订单状态
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
	// 输出链表节点信息,并返回收入
	double printNode() {
		double price = 0;
		for (Node* cur = head->next; cur != nullptr; cur = cur->next) {
			price += cur->order->printOrder();
		}
		return price;
	}
	
private:
	Node *head;  // 链表头节点
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


// 处理预定记录操作
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
		booking(placeMap[order.getPlace()], order);
	}
	else {
		cancelBooking(placeMap[order.getPlace()], order);
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