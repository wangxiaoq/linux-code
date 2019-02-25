#include <list>
#include <iostream>
#include <algorithm>

void radix_sort(int array[], int num, int max, int radix)
{
    int radix_bits = 0;
    while (max > 0) {
        max /= radix;
        radix_bits++;
    }

    std::list<int> bucket[radix];

    for (int j = 0; j < num; j++) {
        bucket[array[j] % radix].push_back(array[j]);
    }
    int current_radix = radix;
    for (int i = 1; i < radix_bits; i++) {
        current_radix *= radix;
        for (int j = 0; j < radix; j++) {
            for (auto it = bucket[j].begin(); it != bucket[j].end(); ) {
                int index = *it % current_radix / (current_radix / radix);
                if (index != j) {
                    int value = *it;
                    it = bucket[j].erase(it);
                    bucket[index].push_back(value);
                } else {
                    ++it;
                }
            }
        }
    }

    int i = 0;
    for (int j = 0; j < radix; j++) {
        for (auto v : bucket[j]) {
            array[i++] = v;
        }
    }
}

int main(int argc, char *argv[])
{
    int array[] = {34, 67, 21, 1, 56, 78, 44, 66, 123, 1};
    radix_sort(array, 10, 123, 10);
    std::for_each(std::begin(array), std::end(array), [](int v)->void{std::cout << v << " ";});
    std::cout << std::endl;

    return 0;
}
