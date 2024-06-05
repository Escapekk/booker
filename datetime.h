#pragma once
#include<string>


// 时间类
class Datetime {
	friend class Order;

public:
	bool operator==(const Datetime& datetime);
	Datetime& operator=(const Datetime& datetime);

	// 解析字符串获得日期时间
	void parseDatetime(const std::string& dateStr, const std::string& timeStr);

	// 判断两个时间区间是否有交集
	bool haveIntersection(const Datetime& datetime);

	// 判断当前时间是否晚于datetime
	bool isLater(const Datetime& datetime);
private:
	int year;  //年
	int month;  //月
	int day;  //日
	int weekday;  //星期几
	int startTime;  //开始时间
	int endTime;  //结束时间

	// 根据日期计算星期几
	int getWeekdayByDate();
};