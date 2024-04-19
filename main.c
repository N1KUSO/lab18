#include "string_.h"
#define ASSERT_STRING(expected, got) assertString(expected, got, \
__FILE__, __FUNCTION__, __LINE__)


void removeNonLetters(char *s) {
    char *endSource = getEndOfString(s);
    char *destination = copyIf(s, endSource, s, isgraph);
    *destination = '\0';
}

void assertString(const char *expected, char *got, char const *fileName, char const *funcName, int line) {
    if (strcmp(expected, got)) {
        fprintf(stderr, "File %s\n", fileName);
        fprintf(stderr, "%s - failed on line %d\n", funcName, line);
        fprintf(stderr, "Expected: \"%s\"\n", expected);
        fprintf(stderr, "Got: \"%s\"\n\n", got);
    } else
        fprintf(stderr, "%s - OK\n", funcName);
}

void removeExtraSpaces(char *s) {
    if (s == NULL || *s == '\0') {
        return;
    }

    char* inputPtr = s;
    char* outputPtr = s;

    while (isspace(*inputPtr)) {
        inputPtr++;
    }

    while (*inputPtr != '\0' && !isspace(*inputPtr)) {
        *outputPtr = *inputPtr;
        inputPtr++;
        outputPtr++;
    }

    while (*inputPtr != '\0') {
        if (isspace(*inputPtr)) {
            while (isspace(*(inputPtr + 1))) {
                inputPtr++;
            }
            *outputPtr = ' ';
            outputPtr++;
        } else {
            *outputPtr = *inputPtr;
            outputPtr++;
        }
        inputPtr++;
    }
    *outputPtr = '\0';
}

void test_removeNonLetters() {
    char s[] = "Hi 12  3  ";
    removeNonLetters(s);
    ASSERT_STRING("Hi123", s);
}

void test_removeExtraSpaces() {
    char s[] = "    Hi     World";
    removeExtraSpaces(s);

    ASSERT_STRING("Hi World", s);

}

int main() {
    test_removeNonLetters();
    test_removeExtraSpaces();
}