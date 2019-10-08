#include "main.h"

int get_pivot_index(int start, int end) {
    int n = end - start + 1;
    int ans = rand();
    return start + (ans % n);
}

void swap(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}