#include <assert.h>
#include "queue.h"

static void test_static_queue(void)
{
    StaticQueue *queue = create_StaticQueue();
    ElemType value;

    assert(queue != NULL);
    assert(is_StaticQueueEmpty(queue) == TRUE);
    assert(dequeue_StaticQueue(queue, &value) == ERROR);
    assert(peek_StaticQueueFront(queue, &value) == ERROR);
    assert(peek_StaticQueueRear(queue, &value) == ERROR);
    for (int i = 0; i < MAXSIZE; ++i) {
        assert(enqueue_StaticQueue(queue, (ElemType)i) == SUCCESS);
    }
    assert(is_StaticQueueFull(queue) == TRUE);
    assert(enqueue_StaticQueue(queue, 'x') == ERROR);
    for (int i = 0; i < MAXSIZE / 2; ++i) {
        assert(dequeue_StaticQueue(queue, &value) == SUCCESS);
        assert(value == (ElemType)i);
    }
    for (int i = MAXSIZE; i < MAXSIZE + MAXSIZE / 2; ++i) {
        assert(enqueue_StaticQueue(queue, (ElemType)i) == SUCCESS);
    }
    assert(get_StaticQueueLength(queue) == MAXSIZE);
    assert(peek_StaticQueueFront(queue, &value) == SUCCESS);
    assert(value == (ElemType)(MAXSIZE / 2));
    assert(peek_StaticQueueRear(queue, &value) == SUCCESS);
    assert(value == (ElemType)(MAXSIZE + MAXSIZE / 2 - 1));
    for (int i = MAXSIZE / 2; i < MAXSIZE + MAXSIZE / 2; ++i) {
        assert(dequeue_StaticQueue(queue, &value) == SUCCESS);
        assert(value == (ElemType)i);
    }
    assert(is_StaticQueueEmpty(queue) == TRUE);
    assert(destroy_StaticQueue(queue) == SUCCESS);
}

static void test_dynamic_queue(void)
{
    DynamicQueue *queue = create_DynamicQueue(2, sizeof(int));
    int input, output;

    assert(queue != NULL);
    assert(is_DynamicQueueEmpty(queue) == TRUE);
    assert(dequeue_DynamicQueue(queue, &output) == ERROR);
    input = 11;
    assert(enqueue_DynamicQueue(queue, &input) == SUCCESS);
    input = 22;
    assert(enqueue_DynamicQueue(queue, &input) == SUCCESS);
    input = 33;
    assert(enqueue_DynamicQueue(queue, &input) == SUCCESS);
    assert(get_DynamicQueueLength(queue) == 3);
    assert(peek_DynamicQueueFront(queue, &output) == SUCCESS);
    assert(output == 11);
    assert(peek_DynamicQueueRear(queue, &output) == SUCCESS);
    assert(output == 33);
    assert(dequeue_DynamicQueue(queue, &output) == SUCCESS);
    assert(output == 11);
    assert(dequeue_DynamicQueue(queue, &output) == SUCCESS);
    assert(output == 22);
    assert(dequeue_DynamicQueue(queue, &output) == SUCCESS);
    assert(output == 33);
    assert(is_DynamicQueueEmpty(queue) == TRUE);
    assert(dequeue_DynamicQueue(queue, &output) == ERROR);

    input = 44;
    assert(enqueue_DynamicQueue(queue, &input) == SUCCESS);
    assert(dequeue_DynamicQueue(queue, &output) == SUCCESS);
    assert(output == 44);
    assert(destroy_DynamicQueue(queue) == SUCCESS);
}

static void test_linked_list(void)
{
    LList *list = create_LinkedList();
    ElemType value;

    assert(list != NULL);
    assert(is_LinkedListEmpty(list) == TRUE);
    assert(dequeue_LinkedList(list, HEAD, &value) == ERROR);
    assert(dequeue_LinkedList(list, TAIL, &value) == ERROR);
    assert(insert_LinkedList(list, 0, 'x') == ERROR);
    assert(insert_LinkedList(list, 1, 'b') == SUCCESS);
    assert(enqueue_LinkedList(list, TAIL, 'c') == SUCCESS);
    assert(enqueue_LinkedList(list, HEAD, 'a') == SUCCESS);
    assert(get_LinkedListLength(list) == 3);
    assert(peek_LinkedList(list, 1, &value) == SUCCESS);
    assert(value == 'a');
    assert(peek_LinkedList(list, 3, &value) == SUCCESS);
    assert(value == 'c');
    assert(peek_LinkedList(list, 0, &value) == ERROR);
    assert(update_LinkedList(list, 2, 'x') == SUCCESS);
    assert(delete_LinkedList(list, 2, &value) == SUCCESS);
    assert(value == 'x');
    assert(delete_LinkedList(list, 3, &value) == ERROR);
    assert(dequeue_LinkedList(list, HEAD, &value) == SUCCESS);
    assert(value == 'a');
    assert(dequeue_LinkedList(list, TAIL, &value) == SUCCESS);
    assert(value == 'c');
    assert(is_LinkedListEmpty(list) == TRUE);
    assert(destroy_LinkedList(list) == SUCCESS);
}

int main(void)
{
    test_static_queue();
    test_dynamic_queue();
    test_linked_list();
    return SUCCESS;
}
