#pragma once
#include "order.h"

class LinkedList {
public:
    // 链表节点：用于存储订单记录
    struct Node {
        Order* order; // 指向订单记录
        Node* next;  // 指向下一个节点

        Node() :order(nullptr), next(nullptr) {}
        Node(const Order& _order) :order(new Order(_order)), next(nullptr) {}
        ~Node() {
            delete order;
        }
    };

    LinkedList();
    ~LinkedList();
    // 判断是否可以新增订单
    bool isConflict(const Order& order);
    // 订单链表新增订单
    void addNode(const Order order);
    // 取消订单，修改订单状态
    bool updateNode(const Order& order);
    // 输出链表节点信息,并返回收入
    double printNode();

private:
    Node* head;  // 链表头节点
};