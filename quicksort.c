#include "main.h"

int get_pivot_index(int start, int end) {
    int n = end - start + 1;
    int ans = rand();
    return  start + ans % n;
}

void swap(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void normal_quick_sort(int arr[], int start, int end) {
    if (start >= end)
        return;

    if (end - start + 1 <= 5) {
        // Insertion sort
        for (int i = start + 1; i <= end; i++) {
            int temp = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > temp) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = temp;
        }
        return;
    }

    int pivot = get_pivot_index(start, end);
    swap(arr, pivot, end);
    pivot = end;

    int i = start - 1;
    for (int j = start; j < end; j++) {
        if (arr[j] < arr[pivot]) {
            i++;
            swap(arr, i, j);
        }
    }
    swap(arr, i + 1, end);
    pivot = i + 1;
    // Partitioning done

    normal_quick_sort(arr, start, pivot - 1);
    normal_quick_sort(arr, pivot + 1, end);
}