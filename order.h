#pragma once
#include "datetime.h"

using namespace std;

// 订单类
class Order {
public:
	Order() {}
	Order(const Order& order);
	bool operator==(const Order& order);
	string& getPlace();
	// 验证订单时间参数是否有效
	bool checkTimeValid();
	// 计算订单价格
	void getPrice();
	// 解析输入获得订单信息
	void parseInput(const string& input);
	// 取消订单并计算违约金
	void updateOrder();
	// 打印订单信息
	double printOrder();
	// 判断两个订单时间是否冲突
	bool isConflict(const Order& order);
	// 判断两个订单起始时间大小
	bool isLater(const Order& order);
	// 判断订单是否取消
	bool checkCancelled();

private:
	string userID; // 用户ID
	Datetime date; // 订单时间
	string place; //场地
	double price = 0;  //订单收入
	bool isCancelled = false; //是否已取消
};