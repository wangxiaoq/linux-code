#include "avl-tree.h"
#include <iostream>
#include <algorithm>

int height(Node *root)
{
    if (root == nullptr) {
        return -1;
    } else {
        return root->height;
    }
}

static Node *single_rotate_with_left(Node *k)
{
    Node *k1 = k->left;
    k->left = k1->right;
    k1->right = k;
    k->height = std::max(height(k->left), height(k->right)) + 1;
    k1->height = std::max(k->height, height(k1->left)) + 1;
    return k1;
}

static Node *single_rotate_with_right(Node *k)
{
    Node *k1 = k->right;
    k->right = k1->left;
    k1->left = k;
    k->height = std::max(height(k->left), height(k->right)) + 1;
    k1->height = std::max(k->height, height(k1->right)) + 1;
    return k1;
}

static Node *double_rotate_with_left(Node *k)
{
    k->left = single_rotate_with_right(k->left);
    return single_rotate_with_left(k);
}

static Node *double_rotate_with_right(Node *k)
{
    k->right = single_rotate_with_left(k->right);
    return single_rotate_with_right(k);
}

Node *insert(Node *root, int val)
{
    if (root == nullptr) {
        root = new Node(val);
    } else if (val > root->val) {
        root->right = insert(root->right, val);
        if (height(root->right) - height(root->left) == 2) {
            if (val > root->right->val) {
                root = single_rotate_with_right(root);
            } else {
                root = double_rotate_with_right(root);
            }
        }
    } else if (val < root->val) {
        root->left = insert(root->left, val);
        if (height(root->right) - height(root->left) == 2) {
            if (val < root->val) {
                root = single_rotate_with_left(root);
            } else {
                root = double_rotate_with_left(root);
            }
        }
    }

    root->height = std::max(height(root->left), height(root->right)) + 1;
    return root;
}

void inorder_traversal(Node *root)
{
    if (root == nullptr) {
        return ;
    }
    inorder_traversal(root->left);
    std::cout << root->val << " ";
    inorder_traversal(root->right);
}

int main(int argc, char *argv[])
{
    Node *tree = nullptr;
    tree = insert(tree, 1);
    tree = insert(tree, 2);
    tree = insert(tree, 3);
    tree = insert(tree, 4);
    tree = insert(tree, 5);
    tree = insert(tree, 6);
    tree = insert(tree, 7);
    tree = insert(tree, 8);

    inorder_traversal(tree);
    std::cout << std::endl;

    std::cout << "height: " << height(tree) << std::endl;
    return 0;
}
