#include <stdio.h>
#include <string.h>

int strMatch(char *str, char *pattern) {
    int strLen = strlen(str);
    int patternLen = strlen(pattern);
    for (int i = 0; i <= strLen - patternLen; i++) {
        int j=0;
        for (j = 0; j < patternLen; j++) {
            if (str[i + j] != pattern[j]) {
                break;
            }
        }
        if (j == patternLen) {
            return i;
        }
    }
    return -1;
}

int main(void) 
{
    char* str = "abmwyriwmywaqi0mwypyr";
    char* pattern = "mwyp";
    int pos=strMatch(str, pattern);
    if (pos != -1) {
        printf("Pattern found at index %d\n", pos);
    } else {
        printf("Pattern not found\n");
    }
    return 0;
}

