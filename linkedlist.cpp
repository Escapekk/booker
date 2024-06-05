#include "LinkedList.h"

LinkedList::LinkedList() :head(new Node()) {}

LinkedList::~LinkedList() {
    Node* cur = head;
    while (cur != nullptr) {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    head = nullptr;
}

// 判断是否可以新增订单
bool LinkedList::isConflict(const Order& order) {
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
void LinkedList::addNode(const Order order) {
    Node* node = new Node(order);
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
bool LinkedList::updateNode(const Order& order) {
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
double LinkedList::printNode() {
    double price = 0;
    for (Node* cur = head->next; cur != nullptr; cur = cur->next) {
        price += cur->order->printOrder();
    }
    return price;
}