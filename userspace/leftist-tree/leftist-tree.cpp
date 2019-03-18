#include "leftist-tree.h"
#include <cmath>
#include <iostream>

int NPL(tree_node *root)
{
    if (root == nullptr) {
        return -1;
    } else {
        return root->npl;
    }
}

tree_node *merge1(tree_node *h1, tree_node *h2);

tree_node *merge(tree_node *h1, tree_node *h2)
{
    if (h1 == nullptr) {
        return h2;
    }
    if (h2 == nullptr) {
        return h1;
    }
    if (h1->val < h2->val) {
        return merge1(h1, h2);
    } else {
        return merge1(h2, h1);
    }
}

tree_node *merge1(tree_node *h1, tree_node *h2)
{
    if (h1->left == nullptr) {
        h1->left = h2;
    } else {
        h1->right = merge(h1->right, h2);
        if (NPL(h1->left) < NPL(h1->left)) {
            tree_node *tmp = h1->left;
            h1->left = h1->right;
            h1->right = tmp;
        }
        h1->npl = NPL(h1->right) + 1;
    }

	return h1;
}

tree_node *insert(tree_node *root, int val)
{
    return merge(root, new tree_node(val));
}

tree_node *delmin(tree_node *root, int &min)
{
    if (root == nullptr) {
        return nullptr;
    } else {
        min = root->val;
        return merge(root->left, root->right);
    }
}

int main(int argc, char *argv[])
{
    tree_node *root = nullptr;
    for (int i = 0; i < 100; i++) {
        root = insert(root, random() % 100);
    }

    for (int i = 0; i < 100; i++) {
        int val = 0;
        root = delmin(root, val);
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
