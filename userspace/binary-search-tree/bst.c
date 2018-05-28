#include <stdio.h>
#include <stdlib.h>

/*
 * bst - binary search tree
 * basic bst operations
 */


/* data structures */
typedef struct _tree_node {
    int data;
    struct _tree_node *left;
    struct _tree_node *right;
} bst_node_t;

/* insert operation */
bst_node_t *insert(int data, bst_node_t *tree)
{
    if (tree == NULL) {
        tree = malloc(sizeof(bst_node_t));
        if (tree == NULL) {
            return NULL;
        }
        tree->data = data;
        tree->left = NULL;
        tree->right = NULL;
    } else if (data > tree->data) {
        tree->right = insert(data, tree->right);
    } else if (data < tree->data) {
        tree->left = insert(data, tree->left);
    } else { /* data == tree->data : nothing to do */

    }

    return tree;
}

/* find min value operation */
bst_node_t *find_min(bst_node_t *tree)
{
    if (tree == NULL) {
        return NULL;
    }

    if (tree->left == NULL) {
        return tree;
    } else {
        return find_min(tree->left);
    }
}

/* find max value operation */
bst_node_t *find_max(bst_node_t *tree)
{
    if (tree == NULL) {
        return NULL;
    }

    if (tree->right == NULL) {
        return tree;
    } else {
        return find_max(tree->right);
    }
}

/* find operation */
bst_node_t *find(int data, bst_node_t *tree)
{
    if (tree == NULL) {
        return NULL;
    } else if (data > tree->data) {
        return find(data, tree->right);
    } else if (data < tree->data) {
        return find(data, tree->left);
    } else {
        return tree;
    }
}

/* 
 * delete operation
 * 
 * node(data) = leaf node : delete directly
 *       has single child : parent point to the child
 *       has two children : 1. find the min value of `node's` right tree
 *                          2. replace the data in `node` with the min value
 *                          3. delete the node contains min value
 */
bst_node_t *delete(int data, bst_node_t *tree)
{
    bst_node_t *tmp = NULL;

    if (tree == NULL) {
        return NULL;
    } else if (data > tree->data) {
        tree->right = delete(data, tree->right);
    } else if (data < tree->data) {
        tree->left = delete(data, tree->left);
    } else { /* data == tree->data */
        if (tree->left &&  tree->right) { /* has two childen */
            tmp = find_min(tree->right);
            tree->data = tmp->data;
            tree->right = delete(tree->data, tree->right);
            free(tmp);
        } else { /* has single child or no child */
            if (tree->left) {
                tmp = tree;
                tree = tree->left;
                free(tmp);
            } else if (tree->right) {
                tmp = tree;
                tree = tree->right;
                free(tmp);
            } else { /* nothing to do */
                free(tree);
                tree = NULL;
            }
        }
    }

    return tree;
}

/* in-order traversal */
void in_order_traversal(bst_node_t *tree)
{
    if (tree == NULL) {
        return ;
    } else {
        in_order_traversal(tree->left);
        printf("%d ", tree->data);
        in_order_traversal(tree->right);
    }
}


int main(int argc, char *argv[])
{
    bst_node_t *tree = NULL;
    tree = insert(1, tree);
    tree = insert(3, tree);
    tree = insert(5, tree);
    tree = insert(0, tree);
    tree = insert(7, tree);
    tree = insert(6, tree);

    in_order_traversal(tree);
    printf("\n");

    tree = delete(6, tree);

    in_order_traversal(tree);
    printf("\n");

    return 0;
}
