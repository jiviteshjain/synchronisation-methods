#include "main.h"

void* multithread_quick_sort(void* inp) {
    args* input = (args*)inp;
    int start = input->start;
    int end = input->end;
    int* arr = input->arr;

    if (start >= end)
        return NULL;

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
        return NULL;
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

    args left_input;
    left_input.start = start;
    left_input.end = pivot - 1;
    left_input.arr = arr;
    
    pthread_t left_tid;
    pthread_create(&left_tid, NULL, multithread_quick_sort, (void*)&left_input);

    args right_input;
    right_input.start = pivot + 1;
    right_input.end = end;
    right_input.arr = arr;

    pthread_t right_tid;
    pthread_create(&right_tid, NULL, multithread_quick_sort, (void*)&right_input);

    pthread_join(left_tid, NULL);
    pthread_join(right_tid, NULL);

    return NULL;
}