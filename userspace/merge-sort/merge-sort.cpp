#include <algorithm>
#include <iostream>

void merge_array(int array[], int start, int mid, int end, int tmp[])
{
    int i = start, j = mid + 1, k = 0;
    for (; i <= mid && j <= end; ) {
        if (array[i] <= array[j]) {
            tmp[k] = array[i];
            i++;
        } else {
            tmp[k] = array[j];
            j++;
        }
        k++;
    }
    while (i <= mid) {
        tmp[k] = array[i];
        k++;
        i++;
    }
    while (j <= end) {
        tmp[k] = array[j];
        k++;
        j++;
    }
    for (i = 0; i < k; i++) {
        array[start+i] = tmp[i];
    }
}

void merge_sort(int array[], int start, int end, int tmp[])
{
    if (start >= end) {
        return;
    }
    int mid = (start + end) / 2;
    merge_sort(array, start, mid, tmp);
    merge_sort(array, mid + 1, end, tmp);
    merge_array(array, start, mid, end, tmp);
}

int main(int argc, char *argv[])
{
    int a[] = {9,4,8,1,3,5,7,2,4};
    int tmp[9];
    merge_sort(a, 0, 8, tmp);

    std::for_each(std::begin(a), std::end(a), [](int p)->void{std::cout << p << " ";});
    std::cout << std::endl;

    return 0;
}
