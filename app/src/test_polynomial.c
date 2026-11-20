#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

static int parse_number(const char **cursor, long double *number)
{
    char *end;

    while (isspace((unsigned char)**cursor)) {
        (*cursor)++;
    }
    if (**cursor == '\0') {
        return 0;
    }

    errno = 0;
    *number = strtold(*cursor, &end);
    if (end == *cursor || errno == ERANGE || !isfinite(*number)) {
        return -1;
    }
    *cursor = end;
    return 1;
}

int main(void)
{
    char input[MAXSIZE + 2];
    char queued_input[MAXSIZE + 1];
    size_t input_length;
    size_t index;
    const char *cursor;
    long double x;
    long double coefficient;
    long double result;
    int parsed;
    StaticQueue *queue;

    printf("Enter x and coefficients from highest power to constant term "
           "(e.g. 2 3 -2 5): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read polynomial input.\n");
        return ERROR;
    }

    input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n') {
        input[--input_length] = '\0';
    } else if (!feof(stdin) && input_length == sizeof(input) - 1) {
        fprintf(stderr, "Input exceeds the queue capacity.\n");
        return ERROR;
    }
    if (input_length > MAXSIZE) {
        fprintf(stderr, "Input exceeds the queue capacity.\n");
        return ERROR;
    }

    queue = create_StaticQueue();
    if (queue == NULL) {
        fprintf(stderr, "Failed to create queue.\n");
        return ERROR;
    }

    for (index = 0; index < input_length; index++) {
        if (enqueue_StaticQueue(queue, input[index]) != SUCCESS) {
            fprintf(stderr, "Failed to enqueue polynomial input.\n");
            destroy_StaticQueue(queue);
            return ERROR;
        }
    }
    for (index = 0; index < input_length; index++) {
        if (dequeue_StaticQueue(queue, &queued_input[index]) != SUCCESS) {
            fprintf(stderr, "Failed to dequeue polynomial input.\n");
            destroy_StaticQueue(queue);
            return ERROR;
        }
    }
    queued_input[input_length] = '\0';
    destroy_StaticQueue(queue);

    cursor = queued_input;
    parsed = parse_number(&cursor, &x);
    if (parsed != 1) {
        fprintf(stderr, "Enter x followed by at least one coefficient.\n");
        return ERROR;
    }

    parsed = parse_number(&cursor, &coefficient);
    if (parsed != 1) {
        fprintf(stderr, "Enter at least one polynomial coefficient.\n");
        return ERROR;
    }
    result = coefficient;

    while ((parsed = parse_number(&cursor, &coefficient)) == 1) {
        result = result * x + coefficient;
        if (!isfinite(result)) {
            fprintf(stderr, "Polynomial result is out of range.\n");
            return ERROR;
        }
    }
    if (parsed < 0) {
        fprintf(stderr, "Invalid number in polynomial input.\n");
        return ERROR;
    }

    printf("P(x) = %.10Lg\n", result);
    return SUCCESS;
}
