#pragma once
#include<string>


// ʱ����
class Datetime {
	friend class Order;

public:
	bool operator==(const Datetime& datetime);
	Datetime& operator=(const Datetime& datetime);

	// �����ַ����������ʱ��
	void parseDatetime(const std::string& dateStr, const std::string& timeStr);

	// �ж�����ʱ�������Ƿ��н���
	bool haveIntersection(const Datetime& datetime);

	// �жϵ�ǰʱ���Ƿ�����datetime
	bool isLater(const Datetime& datetime);
private:
	int year;  //��
	int month;  //��
	int day;  //��
	int weekday;  //���ڼ�
	int startTime;  //��ʼʱ��
	int endTime;  //����ʱ��

	// �������ڼ������ڼ�
	int getWeekdayByDate();
};