#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

void merge(std::vector<int> &iv, std::vector<int> &tmp, int ls, int rs, int re)
{
    int elem_num = re - ls + 1;
    int le = rs - 1;
    int ts = ls;

    while (ls <= le && rs <= re) {
        if (iv[ls] <= iv[rs]) {
            tmp[ts++] = iv[ls++];
        } else {
            tmp[ts++] = iv[rs++];
        }
    }
    while (ls <= le) {
        tmp[ts++] = iv[ls++];
    }
    while (rs <= re) {
        tmp[ts++] = iv[rs++];
    }
    for (int i = elem_num; i > 0; --i, --re) {
        iv[re] = tmp[re];
    }
}

void msort(std::vector<int> &iv, std::vector<int> &tmp, int l, int r)
{
    if (l < r) {
        int m = (l + r) / 2;
        msort(iv, tmp, l, m);
        msort(iv, tmp, m + 1, r);
        merge(iv, tmp, l, m + 1, r);
    }
}

void merge_sort(std::vector<int> &iv)
{
    std::vector<int> t(iv.size());
    msort(iv, t, 0, iv.size() - 1);
}

int main(int argc, char *argv[])
{
    std::vector<int> iv;
    for (int i = 0; i < 100; ++i) {
        iv.push_back(random() % 100);
    }
    merge_sort(iv);
    std::for_each(iv.cbegin(), iv.cend(), [](int v)->void { std::cout << v << " "; });
    std::cout << std::endl;
}
