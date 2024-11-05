/**
 * 设计一个环形队列，用 front 和 rear 分别作为队头和队尾的指针，
 * 另外用一个变量 tag 表示队列是空还是不空。要求设计队列的相关基本运算算法。
 */

#include <iostream>
using namespace std;

template <class T> class CircularQueue {
  T *front, *rear, *data;
  bool tag;
  int size;

public:
  CircularQueue(int size) {
    data = new T[size];
    front = rear = data;
    tag = true;
    this->size = size;
  }
  ~CircularQueue() { delete[] data; }
  bool empty() { return tag; }
  bool full() { return front == rear && !tag; }
  void push(T x) {
    if (full()) {
      cout << "Queue is full!" << endl;
      return;
    }
    *rear = x;
    rear = front + (rear - front + 1) % size;
    tag = false;
  }
  T pop() {
    if (empty()) {
      cout << "Queue is empty!" << endl;
      return T();
    }
    T x = *front;
    front = front + 1 % size;
    if (front == rear)
      tag = true;
    return x;
  }
  T top() {
    if (empty()) {
      cout << "Queue is empty!" << endl;
      return T();
    }
    return *front;
  }
};

int main() {
  CircularQueue<int> q(5);
  q.push(1);
  q.push(2);
  q.push(3);
  q.push(4);
  q.push(5);
  cout << q.empty() << endl;
  cout << q.full() << endl;
  cout << q.top() << endl;
  q.push(6);
  cout << q.pop() << endl;
  return 0;
}