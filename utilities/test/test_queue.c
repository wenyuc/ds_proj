#include "queue.h"

static void test_static_queue(void)
{
    StaticQueue *queue = create_StaticQueue();
    ElemType value;

    CHECK(queue != NULL, "queue != NULL");
    if (queue == NULL) return;
    CHECK(is_StaticQueueEmpty(queue) == TRUE, "is_StaticQueueEmpty(queue) == TRUE");
    CHECK(dequeue_StaticQueue(queue, &value) == ERROR, "dequeue_StaticQueue(queue, &value) == ERROR");
    CHECK(peek_StaticQueueFront(queue, &value) == ERROR, "peek_StaticQueueFront(queue, &value) == ERROR");
    CHECK(peek_StaticQueueRear(queue, &value) == ERROR, "peek_StaticQueueRear(queue, &value) == ERROR");
    for (int i = 0; i < MAXSIZE; ++i) {
        CHECK(enqueue_StaticQueue(queue, (ElemType)i) == SUCCESS, "enqueue_StaticQueue(queue, (ElemType)i) == SUCCESS");
    }
    CHECK(is_StaticQueueFull(queue) == TRUE, "is_StaticQueueFull(queue) == TRUE");
    CHECK(enqueue_StaticQueue(queue, 'x') == ERROR, "enqueue_StaticQueue(queue, 'x') == ERROR");
    for (int i = 0; i < MAXSIZE / 2; ++i) {
        CHECK(dequeue_StaticQueue(queue, &value) == SUCCESS, "dequeue_StaticQueue(queue, &value) == SUCCESS");
        CHECK(value == (ElemType)i, "value == (ElemType)i");
    }
    for (int i = MAXSIZE; i < MAXSIZE + MAXSIZE / 2; ++i) {
        CHECK(enqueue_StaticQueue(queue, (ElemType)i) == SUCCESS, "enqueue_StaticQueue(queue, (ElemType)i) == SUCCESS");
    }
    CHECK(get_StaticQueueLength(queue) == MAXSIZE, "get_StaticQueueLength(queue) == MAXSIZE");
    CHECK(peek_StaticQueueFront(queue, &value) == SUCCESS, "peek_StaticQueueFront(queue, &value) == SUCCESS");
    CHECK(value == (ElemType)(MAXSIZE / 2), "value == (ElemType)(MAXSIZE / 2)");
    CHECK(peek_StaticQueueRear(queue, &value) == SUCCESS, "peek_StaticQueueRear(queue, &value) == SUCCESS");
    CHECK(value == (ElemType)(MAXSIZE + MAXSIZE / 2 - 1), "value == (ElemType)(MAXSIZE + MAXSIZE / 2 - 1)");
    for (int i = MAXSIZE / 2; i < MAXSIZE + MAXSIZE / 2; ++i) {
        CHECK(dequeue_StaticQueue(queue, &value) == SUCCESS, "dequeue_StaticQueue(queue, &value) == SUCCESS");
        CHECK(value == (ElemType)i, "value == (ElemType)i");
    }
    CHECK(is_StaticQueueEmpty(queue) == TRUE, "is_StaticQueueEmpty(queue) == TRUE");
    CHECK(destroy_StaticQueue(queue) == SUCCESS, "destroy_StaticQueue(queue) == SUCCESS");
}

static void test_dynamic_queue(void)
{
    DynamicQueue *queue = create_DynamicQueue(2, sizeof(int));
    int input, output;

    CHECK(queue != NULL, "queue != NULL");
    if (queue == NULL) return;
    CHECK(is_DynamicQueueEmpty(queue) == TRUE, "is_DynamicQueueEmpty(queue) == TRUE");
    CHECK(dequeue_DynamicQueue(queue, &output) == ERROR, "dequeue_DynamicQueue(queue, &output) == ERROR");
    input = 11;
    CHECK(enqueue_DynamicQueue(queue, &input) == SUCCESS, "enqueue_DynamicQueue(queue, &input) == SUCCESS");
    input = 22;
    CHECK(enqueue_DynamicQueue(queue, &input) == SUCCESS, "enqueue_DynamicQueue(queue, &input) == SUCCESS");
    input = 33;
    CHECK(enqueue_DynamicQueue(queue, &input) == SUCCESS, "enqueue_DynamicQueue(queue, &input) == SUCCESS");
    CHECK(get_DynamicQueueLength(queue) == 3, "get_DynamicQueueLength(queue) == 3");
    CHECK(peek_DynamicQueueFront(queue, &output) == SUCCESS, "peek_DynamicQueueFront(queue, &output) == SUCCESS");
    CHECK(output == 11, "output == 11");
    CHECK(peek_DynamicQueueRear(queue, &output) == SUCCESS, "peek_DynamicQueueRear(queue, &output) == SUCCESS");
    CHECK(output == 33, "output == 33");
    CHECK(dequeue_DynamicQueue(queue, &output) == SUCCESS, "dequeue_DynamicQueue(queue, &output) == SUCCESS");
    CHECK(output == 11, "output == 11");
    CHECK(dequeue_DynamicQueue(queue, &output) == SUCCESS, "dequeue_DynamicQueue(queue, &output) == SUCCESS");
    CHECK(output == 22, "output == 22");
    CHECK(dequeue_DynamicQueue(queue, &output) == SUCCESS, "dequeue_DynamicQueue(queue, &output) == SUCCESS");
    CHECK(output == 33, "output == 33");
    CHECK(is_DynamicQueueEmpty(queue) == TRUE, "is_DynamicQueueEmpty(queue) == TRUE");
    CHECK(dequeue_DynamicQueue(queue, &output) == ERROR, "dequeue_DynamicQueue(queue, &output) == ERROR");

    input = 44;
    CHECK(enqueue_DynamicQueue(queue, &input) == SUCCESS, "enqueue_DynamicQueue(queue, &input) == SUCCESS");
    CHECK(dequeue_DynamicQueue(queue, &output) == SUCCESS, "dequeue_DynamicQueue(queue, &output) == SUCCESS");
    CHECK(output == 44, "output == 44");
    CHECK(destroy_DynamicQueue(queue) == SUCCESS, "destroy_DynamicQueue(queue) == SUCCESS");
}

static void test_linked_list(void)
{
    LList *list = create_LinkedList();
    ElemType value;

    CHECK(list != NULL, "list != NULL");
    if (list == NULL) return;
    CHECK(is_LinkedListEmpty(list) == TRUE, "is_LinkedListEmpty(list) == TRUE");
    CHECK(dequeue_LinkedList(list, HEAD, &value) == ERROR, "dequeue_LinkedList(list, HEAD, &value) == ERROR");
    CHECK(dequeue_LinkedList(list, TAIL, &value) == ERROR, "dequeue_LinkedList(list, TAIL, &value) == ERROR");
    CHECK(insert_LinkedList(list, 0, 'x') == ERROR, "insert_LinkedList(list, 0, 'x') == ERROR");
    CHECK(insert_LinkedList(list, 1, 'b') == SUCCESS, "insert_LinkedList(list, 1, 'b') == SUCCESS");
    CHECK(enqueue_LinkedList(list, TAIL, 'c') == SUCCESS, "enqueue_LinkedList(list, TAIL, 'c') == SUCCESS");
    CHECK(enqueue_LinkedList(list, HEAD, 'a') == SUCCESS, "enqueue_LinkedList(list, HEAD, 'a') == SUCCESS");
    CHECK(get_LinkedListLength(list) == 3, "get_LinkedListLength(list) == 3");
    CHECK(peek_LinkedList(list, 1, &value) == SUCCESS, "peek_LinkedList(list, 1, &value) == SUCCESS");
    CHECK(value == 'a', "value == 'a'");
    CHECK(peek_LinkedList(list, 3, &value) == SUCCESS, "peek_LinkedList(list, 3, &value) == SUCCESS");
    CHECK(value == 'c', "value == 'c'");
    CHECK(peek_LinkedList(list, 0, &value) == ERROR, "peek_LinkedList(list, 0, &value) == ERROR");
    CHECK(update_LinkedList(list, 2, 'x') == SUCCESS, "update_LinkedList(list, 2, 'x') == SUCCESS");
    CHECK(delete_LinkedList(list, 2, &value) == SUCCESS, "delete_LinkedList(list, 2, &value) == SUCCESS");
    CHECK(value == 'x', "value == 'x'");
    CHECK(delete_LinkedList(list, 3, &value) == ERROR, "delete_LinkedList(list, 3, &value) == ERROR");
    CHECK(dequeue_LinkedList(list, HEAD, &value) == SUCCESS, "dequeue_LinkedList(list, HEAD, &value) == SUCCESS");
    CHECK(value == 'a', "value == 'a'");
    CHECK(dequeue_LinkedList(list, TAIL, &value) == SUCCESS, "dequeue_LinkedList(list, TAIL, &value) == SUCCESS");
    CHECK(value == 'c', "value == 'c'");
    CHECK(is_LinkedListEmpty(list) == TRUE, "is_LinkedListEmpty(list) == TRUE");
    CHECK(destroy_LinkedList(list) == SUCCESS, "destroy_LinkedList(list) == SUCCESS");
}

int main(void)
{
    test_static_queue();
    test_dynamic_queue();
    test_linked_list();
    return g_fail == 0 ? SUCCESS : ERROR;
}
