/**
 * 输入10个整数构成一个数组。
 * （1）用选择排序、插入排序、冒泡排序算法对其进行降序排序。
 * （2）输入一个整数，用二分法寻找该数是否在数组中。
 */
#include <stdio.h>
#include <stdlib.h>

int array[10];

void selectSort(int array[10])
{
  for (int i = 0; i < 10 - 1; i++)
  {
    int max = i;
    for (int j = i + 1; j < 10; j++)
    {
      if (array[j] > array[max])
      {
        max = j;
      }
    }
    if (max != i)
    {
      int temp = array[max];
      array[max] = array[i];
      array[i] = temp;
    }
  }
}

void insertSort(int array[10])
{
  for (int i = 1; i < 10; i++)
  {
    int temp = array[i];
    int j = i - 1;
    while (j >= 0 && array[j] < temp)
    {
      array[j + 1] = array[j];
      j--;
    }
    array[j + 1] = temp;
  }
}

void bubbleSort(int array[10])
{
  for (int i = 0; i < 10 - 1; i++)
  {
    int flag = 0;
    for (int j = 0; j < 10 - 1 - i; j++)
    {
      if (array[j] < array[j + 1])
      {
        int temp = array[j + 1];
        array[j + 1] = array[j];
        array[j] = temp;
        flag = 1;
      }
    }
    if (flag == 0)
    {
      break;
    }
  }
}

int binarySearch(int array[10], int target)
{
  int left = 0;
  int right = 9;
  while (left <= right)
  {
    int mid = (left + right) / 2;
    if (array[mid] == target)
    {
      return mid;
    }
    else if (array[mid] > target)
    {
      left = mid + 1;
    }
    else
    {
      right = mid - 1;
    }
  }
  return -1;
}

int main()
{
  puts("Input 10 numbers:");
  for (int i = 0; i < 10; i++)
  {
    scanf("%d", &array[i]);
  }

  printf("Choose a sort method:\n"
         "1. Select sort\n"
         "2. Insert sort\n"
         "3. Bubble sort\n");
  int choice;
  scanf("%d", &choice);
  switch (choice)
  {
  case 1:
    selectSort(array);
    break;
  case 2:
    insertSort(array);
    break;
  case 3:
    bubbleSort(array);
    break;
  default:
    puts("Invalid choice.");
    return 0;
  }
  puts("Sorted array:");
  for (int i = 0; i < 10; i++)
  {
    printf("%d ", array[i]);
  }
  puts("");

  printf("Input a number you want to search: ");
  int target;
  scanf("%d", &target);
  int index = binarySearch(array, target);
  if (index == -1)
  {
    printf("%d not found.\n", target);
  }
  else
  {
    printf("%d found at array[%d].\n", target, index);
  }
  return 0;
}