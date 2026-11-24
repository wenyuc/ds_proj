#include "stack.h"

static void test_array_stack(void)
{
    int value;

    CHECK(array_is_empty() == TRUE, "array_is_empty() == TRUE");
    CHECK(array_pop(&value) == ERROR, "array_pop(&value) == ERROR");
    for (int i = 0; i < MAXVEX; ++i) {
        CHECK(array_push(i) == SUCCESS, "array_push(i) == SUCCESS");
    }
    CHECK(array_push(MAXVEX) == ERROR, "array_push(MAXVEX) == ERROR");
    for (int i = MAXVEX - 1; i >= 0; --i) {
        CHECK(array_pop(&value) == SUCCESS, "array_pop(&value) == SUCCESS");
        CHECK(value == i, "value == i");
    }
    CHECK(array_is_empty() == TRUE, "array_is_empty() == TRUE");
}

static void test_static_stack(void)
{
    StaticStack *stack = create_staticStack();
    ElemType value;

    CHECK(stack != NULL, "stack != NULL");
    if (stack == NULL) return;
    CHECK(static_isEmpty(stack) == TRUE, "static_isEmpty(stack) == TRUE");
    CHECK(static_pop(stack, &value) == ERROR, "static_pop(stack, &value) == ERROR");
    for (int i = 0; i < MAXSIZE; ++i) {
        CHECK(static_push(stack, (ElemType)i) == SUCCESS, "static_push(stack, (ElemType)i) == SUCCESS");
    }
    CHECK(static_isFull(stack) == TRUE, "static_isFull(stack) == TRUE");
    CHECK(static_push(stack, 'x') == ERROR, "static_push(stack, 'x') == ERROR");
    CHECK(static_getTop(stack, &value) == SUCCESS, "static_getTop(stack, &value) == SUCCESS");
    CHECK(value == (ElemType)(MAXSIZE - 1), "value == (ElemType)(MAXSIZE - 1)");
    for (int i = MAXSIZE - 1; i >= 0; --i) {
        CHECK(static_pop(stack, &value) == SUCCESS, "static_pop(stack, &value) == SUCCESS");
        CHECK(value == (ElemType)i, "value == (ElemType)i");
    }
    CHECK(static_isEmpty(stack) == TRUE, "static_isEmpty(stack) == TRUE");
    CHECK(destroy_staticStack(stack) == SUCCESS, "destroy_staticStack(stack) == SUCCESS");
}

static void test_dynamic_stack(void)
{
    DynamicStack *stack = create_dynamicStack(2, sizeof(int));
    int input, output;

    CHECK(stack != NULL, "stack != NULL");
    if (stack == NULL) return;
    CHECK(dynamic_isEmpty(stack) == TRUE, "dynamic_isEmpty(stack) == TRUE");
    CHECK(dynamic_pop(stack, &output) == ERROR, "dynamic_pop(stack, &output) == ERROR");
    input = 17;
    CHECK(dynamic_push(stack, &input) == SUCCESS, "dynamic_push(stack, &input) == SUCCESS");
    input = 29;
    CHECK(dynamic_push(stack, &input) == SUCCESS, "dynamic_push(stack, &input) == SUCCESS");
    CHECK(dynamic_isFull(stack) == TRUE, "dynamic_isFull(stack) == TRUE");
    CHECK(dynamic_push(stack, &input) == ERROR, "dynamic_push(stack, &input) == ERROR");
    CHECK(dynamic_getTop(stack, &output) == SUCCESS, "dynamic_getTop(stack, &output) == SUCCESS");
    CHECK(output == 29, "output == 29");
    CHECK(dynamic_pop(stack, &output) == SUCCESS, "dynamic_pop(stack, &output) == SUCCESS");
    CHECK(output == 29, "output == 29");
    CHECK(dynamic_stackSize(stack) == 1, "dynamic_stackSize(stack) == 1");
    CHECK(dynamic_pop(stack, &output) == SUCCESS, "dynamic_pop(stack, &output) == SUCCESS");
    CHECK(output == 17, "output == 17");
    CHECK(dynamic_isEmpty(stack) == TRUE, "dynamic_isEmpty(stack) == TRUE");
    CHECK(destroy_dynamicStack(stack) == SUCCESS, "destroy_dynamicStack(stack) == SUCCESS");
}

static void test_linked_static_stack(void)
{
    LinkedStaticStack *stack = create_linkedStaticStack();
    ElemType value;

    CHECK(stack != NULL, "stack != NULL");
    if (stack == NULL) return;
    CHECK(linkedStatic_isEmpty(stack) == TRUE, "linkedStatic_isEmpty(stack) == TRUE");
    CHECK(linkedStatic_pop(stack, &value) == ERROR, "linkedStatic_pop(stack, &value) == ERROR");
    CHECK(linkedStatic_push(stack, 'a') == SUCCESS, "linkedStatic_push(stack, 'a') == SUCCESS");
    CHECK(linkedStatic_push(stack, 'b') == SUCCESS, "linkedStatic_push(stack, 'b') == SUCCESS");
    CHECK(linkedStatic_stackSize(stack) == 2, "linkedStatic_stackSize(stack) == 2");
    CHECK(linkedStatic_getTop(stack, &value) == SUCCESS, "linkedStatic_getTop(stack, &value) == SUCCESS");
    CHECK(value == 'b', "value == 'b'");
    CHECK(linkedStatic_pop(stack, &value) == SUCCESS, "linkedStatic_pop(stack, &value) == SUCCESS");
    CHECK(value == 'b', "value == 'b'");
    CHECK(linkedStatic_pop(stack, &value) == SUCCESS, "linkedStatic_pop(stack, &value) == SUCCESS");
    CHECK(value == 'a', "value == 'a'");
    CHECK(linkedStatic_isEmpty(stack) == TRUE, "linkedStatic_isEmpty(stack) == TRUE");
    CHECK(destroy_linkedStaticStack(stack) == SUCCESS, "destroy_linkedStaticStack(stack) == SUCCESS");
}

static void test_linked_dynamic_stack(void)
{
    LinkedDynamicStack *stack = create_linkedDynamicStack(1, sizeof(int));
    int input, output;

    CHECK(stack != NULL, "stack != NULL");
    if (stack == NULL) return;
    CHECK(linkedDynamic_isEmpty(stack) == TRUE, "linkedDynamic_isEmpty(stack) == TRUE");
    CHECK(linkedDynamic_pop(stack, &output) == ERROR, "linkedDynamic_pop(stack, &output) == ERROR");
    input = 42;
    CHECK(linkedDynamic_push(stack, &input) == SUCCESS, "linkedDynamic_push(stack, &input) == SUCCESS");
    input = 84;
    CHECK(linkedDynamic_push(stack, &input) == SUCCESS, "linkedDynamic_push(stack, &input) == SUCCESS");
    CHECK(linkedDynamic_stackSize(stack) == 2, "linkedDynamic_stackSize(stack) == 2");
    CHECK(linkedDynamic_getTop(stack, &output) == SUCCESS, "linkedDynamic_getTop(stack, &output) == SUCCESS");
    CHECK(output == 84, "output == 84");
    CHECK(linkedDynamic_pop(stack, &output) == SUCCESS, "linkedDynamic_pop(stack, &output) == SUCCESS");
    CHECK(output == 84, "output == 84");
    CHECK(linkedDynamic_pop(stack, &output) == SUCCESS, "linkedDynamic_pop(stack, &output) == SUCCESS");
    CHECK(output == 42, "output == 42");
    CHECK(linkedDynamic_isEmpty(stack) == TRUE, "linkedDynamic_isEmpty(stack) == TRUE");
    CHECK(destroy_linkedDynamicStack(stack) == SUCCESS, "destroy_linkedDynamicStack(stack) == SUCCESS");
}

int main(void)
{
    test_array_stack();
    test_static_stack();
    test_dynamic_stack();
    test_linked_static_stack();
    test_linked_dynamic_stack();
    return g_fail == 0 ? SUCCESS : ERROR;
}
