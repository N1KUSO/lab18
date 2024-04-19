#include "string_.h"
#define ASSERT_STRING(expected, got) assertString(expected, got, \
__FILE__, __FUNCTION__, __LINE__)

typedef struct WordDescriptor{
    char *begin;
    char *end;
} WordDescriptor;

char _stringBuffer[MAX_STRING_SIZE]; // Буфер для временного хранения строки

void copyToBuffer(const char *source) {
    copy(source, getEndOfString(source), _stringBuffer);
}

void copyFromBuffer(char *destination) {
    copy(_stringBuffer, getEndOfString(_stringBuffer), destination);
}


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

void replace(char *source, char *w1, char *w2) {
    size_t w1Size = strlen_(w1);
    size_t w2Size = strlen_(w2);
    WordDescriptor word1 = {w1, w1 + w1Size};
    WordDescriptor word2 = {w2, w2 + w2Size};
    char *readPtr, *recPtr;

    if (w1Size >= w2Size) {
        readPtr = source;
        recPtr = source;
    } else {
        copyToBuffer(source);
        readPtr = _stringBuffer;
        recPtr = source;
    }

    while (*readPtr != '\0') {
        if (find(readPtr, getEndOfString(readPtr), *w1) == readPtr) {
            int match = 1;
            for (int i = 0; i < w1Size; i++) {
                if (*(readPtr + i) != *(word1.begin + i)) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                copy(word2.begin, word2.end, recPtr);
                readPtr += w1Size;
                recPtr += w2Size;
            } else {
                *recPtr = *readPtr;
                readPtr++;
                recPtr++;
            }
        } else {
            *recPtr = *readPtr;
            readPtr++;
            recPtr++;
        }
    }

    *recPtr = '\0';

    if (w1Size < w2Size) {
        copyFromBuffer(source);
    }
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

void test_replace() {
    char s[] = "hello world";
    char w1[] = "hello";
    char w2[] = "hi";
    replace(s, w1, w2);

    ASSERT_STRING("hi world", s);
}

int main() {
    test_removeNonLetters();
    test_removeExtraSpaces();
    test_replaceDigitsWithSpaces();
    test_replace();
}