#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

void perc_down(std::vector<int> &iv, int i, int size)
{
    int j = 0;
    int child = 0;
    int tmp = iv[i];
    for (j = i; j * 2 + 1 < size; j = child) {
        child = j * 2 + 1;
        if (child + 1 < size && iv[child + 1] > iv[child]) {
            child++;
        }
        if (iv[child] > tmp) {
            iv[j] = iv[child];
        } else {
            break;
        }
    }
    iv[j] = tmp;
}

void heap_sort(std::vector<int> &iv)
{
    for (int i = iv.size() / 2; i >= 0; --i) {
        perc_down(iv, i, iv.size());
    }

    for (int i = iv.size() - 1; i > 0; --i) {
        using std::swap;
        swap(iv[0], iv[i]);
        perc_down(iv, 0, i);
    }
}

int main(int argc, char *argv[])
{
    std::vector<int> iv;
    for (int i = 0; i < 100; ++i) {
        iv.push_back(random() % 100);
    }

    heap_sort(iv);

    for_each(iv.cbegin(), iv.cend(), [](int v)->void { std::cout << v << " "; });
    std::cout << std::endl;
    return 0;
}
