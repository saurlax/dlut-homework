/**
 * 创建一个单向链表存储Point，链表中的每个结点保存Point的x与y（均为int类型）。
 * （1）创建该链表，依次存入点[1,2] [2,3] [3,4] [1,4]
 * （2）输出该链表所有结点数据
 * （3）查找所有x+y=5的结点，并输出这些结点的x与y
 * 练习链表的创建、输出、查找
 */
#include <stdio.h>
#include <stdlib.h>

struct Point
{
  int x;
  int y;
};

struct Node
{
  struct Point point;
  struct Node *next;
};

struct Node *createNode(struct Point point)
{
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
  node->point = point;
  node->next = NULL;
  return node;
}

int main()
{
  struct Node *p;
  struct Point point1 = {1, 2};
  struct Point point2 = {2, 3};
  struct Point point3 = {3, 4};
  struct Point point4 = {1, 4};

  struct Node *head = createNode(point1);
  struct Node *node2 = createNode(point2);
  struct Node *node3 = createNode(point3);
  struct Node *node4 = createNode(point4);

  head->next = node2;
  node2->next = node3;
  node3->next = node4;

  p = head;
  puts("All points:");
  while (p != NULL)
  {
    printf("[%d, %d]\n", p->point.x, p->point.y);
    p = p->next;
  }
  puts("All points where x + y = 5:");
  p = head;
  while (p != NULL)
  {
    if (p->point.x + p->point.y == 5)
    {
      printf("[%d, %d]\n", p->point.x, p->point.y);
    }
    p = p->next;
  }
  return 0;
}