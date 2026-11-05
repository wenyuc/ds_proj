#include <stdio.h>

//顺序查找函数
int sequential_search(int *arr, int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i; // 返回找到的索引
        }
    }
    return -1; // 未找到返回-1
}

//二分查找函数
int binary_search(int *arr, int n, int target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid; // 返回找到的索引
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; // 未找到返回-1
}

int main(void){
    int arr[] = {15,54,76,6,9,45,12}; //查找表
    int len = sizeof(arr) / sizeof(arr[0]); // 数组长度

    int target = 45; // 要查找的目标值
    int seq_index = sequential_search(arr, len, target);
    printf("顺序查找结果: %d\n", seq_index);

    int arr_sorted[] = {2,13,22,38,46,58,66,77,85,98};
    int len_sorted = sizeof(arr_sorted) / sizeof(arr_sorted[0]); // 数组长度
    int bin_index = binary_search(arr_sorted, len_sorted, target);
    printf("二分查找结果: %d\n", bin_index);
    
    return 0;
}