/**
 * 用递归实现二分查找（函数递归）
 */
#include <stdio.h>

int binarySearch(int arr[], int low, int high, int key)
{
  if (low > high)
  {
    return -1;
  }
  int mid = (low + high) / 2;
  if (arr[mid] == key)
  {
    return mid;
  }
  else if (arr[mid] > key)
  {
    return binarySearch(arr, low, mid - 1, key);
  }
  else
  {
    return binarySearch(arr, mid + 1, high, key);
  }
}

int arr[] = {1, 4, 5, 6, 8, 9, 16, 19, 24};

int main()
{
  int key;
  puts("Please input the number you want to search:");
  scanf("%d", &key);
  int index = binarySearch(arr, 0, 8, key);
  if (index == -1)
  {
    puts("Not found!");
  }
  else
  {
    printf("Found! The index is %d\n", index);
  }
  return 0;
}