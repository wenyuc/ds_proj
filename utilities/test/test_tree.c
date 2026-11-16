#include <assert.h>
#include <stddef.h>
#include "tree.h"

static void check_binary_tree(BiTree tree)
{
    assert(tree != NULL && tree->data == 'A');
    assert(tree->lchild != NULL && tree->lchild->data == 'B');
    assert(tree->rchild != NULL && tree->rchild->data == 'C');
    assert(tree->lchild->lchild != NULL && tree->lchild->lchild->data == 'D');
    assert(tree->lchild->rchild != NULL && tree->lchild->rchild->data == 'E');
    assert(tree->lchild->lchild->lchild->data == 'H');
    assert(tree->lchild->lchild->lchild->rchild->data == 'K');
    assert(tree->lchild->lchild->lchild->rchild->lchild == NULL);
    assert(tree->lchild->lchild->lchild->rchild->rchild == NULL);
    assert(tree->rchild->lchild->data == 'F');
    assert(tree->rchild->rchild->data == 'G');
    assert(tree->rchild->lchild->lchild->data == 'I');
    assert(tree->rchild->rchild->rchild->data == 'J');
    assert(tree->lchild->lchild->lchild->lchild == NULL);
}

static void check_threaded_tree(ThreadTree head)
{
    static const char expected[] = "HKDBEAIFCGJ";
    ThreadTree current = head->lchild;

    while (current->ltag == 0) {
        current = current->lchild;
    }
    for (size_t i = 0; i < sizeof(expected) - 1; ++i) {
        assert(current != head);
        assert(current->data == expected[i]);
        if (current->rtag == 1) {
            current = current->rchild;
        } else {
            current = current->rchild;
            while (current->ltag == 0) {
                current = current->lchild;
            }
        }
    }
    assert(current == head);
}

int main(void)
{
    BiTree tree = create_BiTree();
    assert(tree != NULL);
    check_binary_tree(tree);
    assert(destroy_BiTree(tree) == SUCCESS);

    ThreadTree tree_to_thread = create_ThreadTree();
    ThreadTree head = NULL;
    assert(tree_to_thread != NULL);
    assert(inorder_Threading(&head, tree_to_thread) == SUCCESS);
    assert(head != NULL);
    check_threaded_tree(head);
    assert(destroy_ThreadTree(head) == SUCCESS);
    return SUCCESS;
}
