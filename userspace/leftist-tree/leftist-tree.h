#ifndef LEFTIST_TREE_H
#define LEFTIST_TREE

struct tree_node {
    int val;
    int npl;
    tree_node *left;
    tree_node *right;

    tree_node(int v)
        : val(v), npl(0), left(nullptr), right(nullptr) {}
};

#endif
