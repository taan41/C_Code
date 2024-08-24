#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void improved_selection(int arr[], int n, int *steps, int *comp, int *swaps);
void bubble_sort(int arr[], int n, int *steps, int *comp, int *swaps);
void merge_sort(int arr[], int l, int r, int *steps, int *comp, int *swaps);
void quick_sort(int arr[], int l, int r, int *steps, int *comp, int *swaps);
int binary_search(int arr[], int arr_size, int target);

int main() {
    int arr[100], sort_mode = -1;

    // Chon thuat toan
    while(sort_mode < 0 || sort_mode > 3) {
        printf(
            "0. Improved selection sort\n"
            "1. Bubble sort\n"
            "2. Merge sort\n"
            "3. Quick sort\n"
            "Nhap thuat toan sap xep: ");
        scanf("%d", &sort_mode);
    }

    // Gan gia tri ngau nhien vao cac phan tu mang
    srand(time(NULL));
    printf("Cac phan tu mang:");
    for(int i = 0; i < 100; i++) {
        arr[i] = rand() % 1001;
        
        if(i % 10 == 0) printf("\n");
        printf("%-5d ", arr[i]);
    }
    printf("\n\n");

    // Sap xep mang
    int steps = 0, comparisons = 0, swaps = 0;
    switch(sort_mode) {
        case 0:
            improved_selection(arr, 100, &steps, &comparisons, &swaps);
            break;
        case 1:
            bubble_sort(arr, 100, &steps, &comparisons, &swaps);
            break;
        case 2:
            merge_sort(arr, 0, 99, &steps, &comparisons, &swaps);
            break;
        case 3:
            quick_sort(arr, 0, 99, &steps, &comparisons, &swaps);
            break;
    }

    // Hien thi mang
    printf("Sau khi sap xep:");
    for(int i = 0; i < 100; i++) {
        if(i % 10 == 0) printf("\n");
        printf("%-5d ", arr[i]);
    }
    printf("\n\nSteps: %d\nComparisons: %d\nSwaps: %d\n", steps, comparisons, swaps);

    // Nhap va hien thi so can tim
    int target = -1, target_pos = -1;
    while(target < 0 || target > 1000) {
        printf("Nhap 1 so tu 0 den 1000: ");
        scanf("%d", &target);
    }
    target_pos = binary_search(arr, 100, target);
    if(target_pos == -1) printf("Khong tim thay %d\n", target);
    else printf("Tim thay %d, xuat hien dau tien tai vi tri %d\n", target, target_pos);

    return 0;
}

void improved_selection(int arr[], int n, int *steps, int *comp, int *swaps) {
    int min, max, temp;

    for(int i = 0; i <= (n - 1) / 2; i++) {
        min = i;
        max = n - 1 - i;
        int isSorted = 1;

        for(int j = i; j < n - i; j++, (*steps)++, (*comp) += 2) {
            if(arr[j] < arr[min]) {
                min = j;
                isSorted = 0;
            }
            if(arr[j] > arr[max]) {
                max = j;
                isSorted = 0;
            }
        }

        if(isSorted) break;

        if(min != i) {
            temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;

            if(max == i) max = min;

            (*swaps)++;
        }

        if(max != (n - 1 - i)) {
            temp = arr[n - 1 - i];
            arr[n - 1 - i] = arr[max];
            arr[max] = temp;

            (*swaps)++;
        }
    }
}

void bubble_sort(int arr[], int n, int *steps, int *comp, int *swaps) {
    int temp;

    for(int i = 0; i < n - 1; i++) {
        int isSorted = 1;

        for(int j = 0; j < n - 1 - i; j++, (*steps)++, (*comp)++) {
            if(arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                (*swaps)++;

                isSorted = 0;
            }
        }

        if(isSorted) break;
    }
}

void merge_sort_merge(int arr[], int l, int m, int r, int *steps, int *comp, int *swaps) {
    int l_size = m - l + 1, r_size = r - m;
    int l_arr[l_size], r_arr[r_size];

    for(int i = 0; i < l_size; i++, (*steps)++) l_arr[i] = arr[l + i];
    for(int i = 0; i < r_size; i++, (*steps)++) r_arr[i] = arr[m + i + 1];

    int i = 0, j = 0, k;
    for(k = l; i < l_size && j < r_size; k++, (*steps)++, (*comp)++) {
        if(l_arr[i] < r_arr[j]) arr[k] = l_arr[i++];
        else arr[k] = r_arr[j++];
    }

    while(i < l_size) {
        arr[k++] = l_arr[i++];
        (*steps)++;
    }

    while(j < r_size) {
        arr[k++] = r_arr[j++];
        (*steps)++;
    }
}

void merge_sort(int arr[], int l, int r, int *steps, int *comp, int *swaps) {
    (*comp)++;
    if(l < r) {
        int m = (l + r) / 2;
        merge_sort(arr, l, m, steps, comp, swaps);
        merge_sort(arr, m + 1, r, steps, comp, swaps);
        merge_sort_merge(arr, l, m, r, steps, comp, swaps);
    }
}

int quick_sort_partition(int arr[], int l, int r, int *steps, int *comp, int *swaps) {
    int pivot = arr[r];
    int i = l - 1;
    int temp;

    for(int j = l; j < r; j++, (*steps)++, (*comp)++) {
        if(arr[j] <= pivot) {
            i++;

            temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;

            (*swaps)++;
        }
    }

    temp = arr[i + 1];
    arr[i + 1] = arr[r];
    arr[r] = temp;

    (*swaps)++;

    return i + 1;
}

void quick_sort(int arr[], int l, int r, int *steps, int *comp, int *swaps) {
    (*comp)++;
    if(l < r) {
        int pivot_pos = quick_sort_partition(arr, l, r, steps, comp, swaps);

        quick_sort(arr, l, pivot_pos - 1, steps, comp, swaps);
        quick_sort(arr, pivot_pos + 1, r, steps, comp, swaps);
    }
}

int binary_search(int arr_size, int target) {
    int l = 0, r = arr_size - 1, result = -1;

    while(l <= r) {
        int mid = (l + r) / 2;

        if(arr[mid] == target) {
            result = mid;
            r = mid - 1;
        }
        else if (arr[mid] < target) l = mid + 1;
        else r = mid - 1;
    }

    return result;
}