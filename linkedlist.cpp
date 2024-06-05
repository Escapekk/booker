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

// �ж��Ƿ������������
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

// ����������������
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

// ȡ���������޸Ķ���״̬
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

// �������ڵ���Ϣ,����������
double LinkedList::printNode() {
    double price = 0;
    for (Node* cur = head->next; cur != nullptr; cur = cur->next) {
        price += cur->order->printOrder();
    }
    return price;
}