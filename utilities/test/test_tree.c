#include <stddef.h>
#include "tree.h"

static void check_binary_tree(BiTree tree)
{
    int valid = tree != NULL && tree->data == 'A' &&
                tree->lchild != NULL && tree->lchild->data == 'B' &&
                tree->rchild != NULL && tree->rchild->data == 'C' &&
                tree->lchild->lchild != NULL && tree->lchild->lchild->data == 'D' &&
                tree->lchild->rchild != NULL && tree->lchild->rchild->data == 'E' &&
                tree->lchild->lchild->lchild != NULL &&
                tree->lchild->lchild->lchild->data == 'H' &&
                tree->lchild->lchild->lchild->rchild != NULL &&
                tree->lchild->lchild->lchild->rchild->data == 'K' &&
                tree->lchild->lchild->lchild->rchild->lchild == NULL &&
                tree->lchild->lchild->lchild->rchild->rchild == NULL &&
                tree->rchild->lchild != NULL && tree->rchild->lchild->data == 'F' &&
                tree->rchild->rchild != NULL && tree->rchild->rchild->data == 'G' &&
                tree->rchild->lchild->lchild != NULL &&
                tree->rchild->lchild->lchild->data == 'I' &&
                tree->rchild->rchild->rchild != NULL &&
                tree->rchild->rchild->rchild->data == 'J' &&
                tree->lchild->lchild->lchild->lchild == NULL;
    CHECK(valid, "binary tree structure");
}

static void check_threaded_tree(ThreadTree head)
{
    static const char expected[] = "HKDBEAIFCGJ";
    CHECK(head != NULL, "threaded tree head exists");
    if (head == NULL) return;
    ThreadTree current = head->lchild;

    while (current != NULL && current->ltag == 0) {
        current = current->lchild;
    }
    for (size_t i = 0; i < sizeof(expected) - 1; ++i) {
        CHECK(current != NULL && current != head, "threaded traversal has next node");
        if (current == NULL || current == head) return;
        CHECK(current->data == expected[i], "current->data == expected[i]");
        if (current->rtag == 1) {
            current = current->rchild;
        } else {
            current = current->rchild;
            while (current != NULL && current != head && current->ltag == 0) {
                current = current->lchild;
            }
        }
    }
    CHECK(current == head, "current == head");
}

int main(void)
{
    BiTree tree = create_BiTree();
    CHECK(tree != NULL, "tree != NULL");
    if (tree == NULL) return 1;
    check_binary_tree(tree);
    CHECK(destroy_BiTree(tree) == SUCCESS, "destroy_BiTree(tree) == SUCCESS");

    ThreadTree tree_to_thread = create_ThreadTree();
    ThreadTree head = NULL;
    CHECK(tree_to_thread != NULL, "tree_to_thread != NULL");
    if (tree_to_thread == NULL) return 1;
    Status status = inorder_Threading(&head, tree_to_thread);
    CHECK(status == SUCCESS, "inorder_Threading(&head, tree_to_thread) == SUCCESS");
    if (status != SUCCESS) return 1;
    CHECK(head != NULL, "head != NULL");
    if (head == NULL) return 1;
    check_threaded_tree(head);
    CHECK(destroy_ThreadTree(head) == SUCCESS, "destroy_ThreadTree(head) == SUCCESS");
    return g_fail == 0 ? SUCCESS : ERROR;
}
