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

void replaceDigitsWithSpaces(char *s) {
    if (s == NULL || *s == '\0') {
        return;
    }

    char result[MAX_STRING_SIZE];
    int resultIndex = 0;


    for (int i = 0; s[i] != '\0'; i++) {
        if (isdigit(s[i])) {
            int numSpaces = s[i] - '0';
            for (int j = 0; j < numSpaces; j++) {
                result[resultIndex] = ' ';
                resultIndex++;
            }
        } else {
            result[resultIndex] = s[i];
            resultIndex++;
        }
    }

    result[resultIndex] = '\0';

    resultIndex = 0;
    while (result[resultIndex] != '\0') {
        s[resultIndex] = result[resultIndex];
        resultIndex++;
    }
    s[resultIndex] = '\0';
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

void test_replaceDigitsWithSpaces() {
    char s[] = "H2e3llo1Wor1ld";
    replaceDigitsWithSpaces(s);

    ASSERT_STRING("H  e   llo Wor ld", s);
}

int main() {
    test_removeNonLetters();
    test_removeExtraSpaces();
    test_replaceDigitsWithSpaces();
}