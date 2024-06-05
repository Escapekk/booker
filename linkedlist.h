#pragma once
#include "order.h"

class LinkedList {
public:
    // ����ڵ㣺���ڴ洢������¼
    struct Node {
        Order* order; // ָ�򶩵���¼
        Node* next;  // ָ����һ���ڵ�

        Node() :order(nullptr), next(nullptr) {}
        Node(const Order& _order) :order(new Order(_order)), next(nullptr) {}
        ~Node() {
            delete order;
        }
    };

    LinkedList();
    ~LinkedList();
    // �ж��Ƿ������������
    bool isConflict(const Order& order);
    // ����������������
    void addNode(const Order order);
    // ȡ���������޸Ķ���״̬
    bool updateNode(const Order& order);
    // �������ڵ���Ϣ,����������
    double printNode();

private:
    Node* head;  // ����ͷ�ڵ�
};