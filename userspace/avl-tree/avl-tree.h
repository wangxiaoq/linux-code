#ifndef AVL_TREE_H
#define AVL_TREE_H

struct Node {
    int val;
    int height;
    Node *left;
    Node *right;
    Node(int v, int h = 0)
        : val(v), height(h), left(nullptr), right(nullptr) {}
};

int height(Node *root);
Node *insert(Node *root, int val);
void preorder_traversal(Node *root);

#endif
