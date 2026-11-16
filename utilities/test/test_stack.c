#include <assert.h>
#include "stack.h"

static void test_array_stack(void)
{
    int value;

    assert(array_is_empty() == TRUE);
    assert(array_pop(&value) == ERROR);
    for (int i = 0; i < MAXVEX; ++i) {
        assert(array_push(i) == SUCCESS);
    }
    assert(array_push(MAXVEX) == ERROR);
    for (int i = MAXVEX - 1; i >= 0; --i) {
        assert(array_pop(&value) == SUCCESS);
        assert(value == i);
    }
    assert(array_is_empty() == TRUE);
}

static void test_static_stack(void)
{
    StaticStack *stack = create_staticStack();
    ElemType value;

    assert(stack != NULL);
    assert(static_isEmpty(stack) == TRUE);
    assert(static_pop(stack, &value) == ERROR);
    for (int i = 0; i < MAXSIZE; ++i) {
        assert(static_push(stack, (ElemType)i) == SUCCESS);
    }
    assert(static_isFull(stack) == TRUE);
    assert(static_push(stack, 'x') == ERROR);
    assert(static_getTop(stack, &value) == SUCCESS);
    assert(value == (ElemType)(MAXSIZE - 1));
    for (int i = MAXSIZE - 1; i >= 0; --i) {
        assert(static_pop(stack, &value) == SUCCESS);
        assert(value == (ElemType)i);
    }
    assert(static_isEmpty(stack) == TRUE);
    assert(destroy_staticStack(stack) == SUCCESS);
}

static void test_dynamic_stack(void)
{
    DynamicStack *stack = create_dynamicStack(2, sizeof(int));
    int input, output;

    assert(stack != NULL);
    assert(dynamic_isEmpty(stack) == TRUE);
    assert(dynamic_pop(stack, &output) == ERROR);
    input = 17;
    assert(dynamic_push(stack, &input) == SUCCESS);
    input = 29;
    assert(dynamic_push(stack, &input) == SUCCESS);
    assert(dynamic_isFull(stack) == TRUE);
    assert(dynamic_push(stack, &input) == ERROR);
    assert(dynamic_getTop(stack, &output) == SUCCESS);
    assert(output == 29);
    assert(dynamic_pop(stack, &output) == SUCCESS);
    assert(output == 29);
    assert(dynamic_stackSize(stack) == 1);
    assert(dynamic_pop(stack, &output) == SUCCESS);
    assert(output == 17);
    assert(dynamic_isEmpty(stack) == TRUE);
    assert(destroy_dynamicStack(stack) == SUCCESS);
}

static void test_linked_static_stack(void)
{
    LinkedStaticStack *stack = create_linkedStaticStack();
    ElemType value;

    assert(stack != NULL);
    assert(linkedStatic_isEmpty(stack) == TRUE);
    assert(linkedStatic_pop(stack, &value) == ERROR);
    assert(linkedStatic_push(stack, 'a') == SUCCESS);
    assert(linkedStatic_push(stack, 'b') == SUCCESS);
    assert(linkedStatic_stackSize(stack) == 2);
    assert(linkedStatic_getTop(stack, &value) == SUCCESS);
    assert(value == 'b');
    assert(linkedStatic_pop(stack, &value) == SUCCESS);
    assert(value == 'b');
    assert(linkedStatic_pop(stack, &value) == SUCCESS);
    assert(value == 'a');
    assert(linkedStatic_isEmpty(stack) == TRUE);
    assert(destroy_linkedStaticStack(stack) == SUCCESS);
}

static void test_linked_dynamic_stack(void)
{
    LinkedDynamicStack *stack = create_linkedDynamicStack(1, sizeof(int));
    int input, output;

    assert(stack != NULL);
    assert(linkedDynamic_isEmpty(stack) == TRUE);
    assert(linkedDynamic_pop(stack, &output) == ERROR);
    input = 42;
    assert(linkedDynamic_push(stack, &input) == SUCCESS);
    input = 84;
    assert(linkedDynamic_push(stack, &input) == SUCCESS);
    assert(linkedDynamic_stackSize(stack) == 2);
    assert(linkedDynamic_getTop(stack, &output) == SUCCESS);
    assert(output == 84);
    assert(linkedDynamic_pop(stack, &output) == SUCCESS);
    assert(output == 84);
    assert(linkedDynamic_pop(stack, &output) == SUCCESS);
    assert(output == 42);
    assert(linkedDynamic_isEmpty(stack) == TRUE);
    assert(destroy_linkedDynamicStack(stack) == SUCCESS);
}

int main(void)
{
    test_array_stack();
    test_static_stack();
    test_dynamic_stack();
    test_linked_static_stack();
    test_linked_dynamic_stack();
    return SUCCESS;
}
