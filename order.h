#pragma once
#include "datetime.h"

using namespace std;

// ������
class Order {
public:
	Order() {}
	Order(const Order& order);
	bool operator==(const Order& order);
	string& getPlace();
	// ��֤����ʱ������Ƿ���Ч
	bool checkTimeValid();
	// ���㶩���۸�
	void getPrice();
	// ���������ö�����Ϣ
	void parseInput(const string& input);
	// ȡ������������ΥԼ��
	void updateOrder();
	// ��ӡ������Ϣ
	double printOrder();
	// �ж���������ʱ���Ƿ��ͻ
	bool isConflict(const Order& order);
	// �ж�����������ʼʱ���С
	bool isLater(const Order& order);
	// �ж϶����Ƿ�ȡ��
	bool checkCancelled();

private:
	string userID; // �û�ID
	Datetime date; // ����ʱ��
	string place; //����
	double price = 0;  //��������
	bool isCancelled = false; //�Ƿ���ȡ��
};