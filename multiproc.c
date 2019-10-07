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

void multiproc_quick_sort(int arr[], int start, int end) {
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
    pivot = i+1;
    // PARTITIONING DONE

    pid_t left_pid = fork();
    if (left_pid < 0) {
        // IN PARENT, CHILD NOT CREATED
        perror("Could not fork");
        return;
    } else if (left_pid == 0) {
        // IN LEFT CHILD, SORT LEFT HALF
        multiproc_quick_sort(arr, start, pivot - 1);
        exit(0);
    } else {
    // IN PARENT, CHILD WOULD NEVER REACH HERE

        pid_t right_pid = fork();
        if (right_pid < 0) {
            // IN PARENT, CHILD NOT CREATED
            perror("Could not fork");
            return;
        } else if (right_pid == 0) {
            // IN RIGHT CHILD, SORT RIGHT HALF
            multiproc_quick_sort(arr, pivot + 1, end);
            exit(0);
        } else {
            // IN PARENT, NO CHILD REACHES HERE
            int w_st_left, w_st_right;
            waitpid(left_pid, &w_st_left, 0);
            waitpid(right_pid, &w_st_right, 0);
        }
    }
}

int main(void) {
    key_t mem_key = IPC_PRIVATE;
    int shm_id = shmget(mem_key, sizeof(int) * 15, IPC_CREAT | 0666);
    int* arr =  (int*)shmat(shm_id, NULL, 0);

    for (int i = 0; i < 15; i++) {
        scanf("%d", &arr[i]);
    }
    multiproc_quick_sort(arr, 0, 14);
    for (int i = 0; i < 15; i++) {
        printf("%d ", arr[i]);
    }
}