#include "string_.h"
#include <assert.h>

#define ASSERT_STRING(expected, got) assertString(expected, got, \
__FILE__, __FUNCTION__, __LINE__)

typedef struct WordDescriptor {
    char *begin;
    char *end;
} WordDescriptor;

typedef struct BagOfWords {
    WordDescriptor words[MAX_N_WORDS_IN_STRING];
    size_t size;
} BagOfWords;

typedef enum WordBeforeFirstWordWithAReturnCode {
    FIRST_WORD_WITH_A,
    NOT_FOUND_A_WORD_WITH_A,
    WORD_FOUND,
    EMPTY_STRING
} WordBeforeFirstWordWithAReturnCode;

BagOfWords _bag;
BagOfWords _bag2;

char _stringBuffer[MAX_STRING_SIZE];

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

    char *inputPtr = s;
    char *outputPtr = s;

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

int areWordsEqual(WordDescriptor w1, WordDescriptor w2) {
    char *ptr1 = w1.begin;
    char *ptr2 = w2.begin;

    while (ptr1 != w1.end && ptr2 != w2.end) {
        if (*ptr1 != *ptr2) {
            return 0;
        }
        ptr1++;
        ptr2++;
    }

    return ptr1 == w1.end && ptr2 == w2.end;
}

int areWordsLexicographicallyOrdered(char *sentence) {
    WordDescriptor words[MAX_WORD_SIZE];
    int wordCount = 0;

    char *ptr = sentence;
    while (*ptr != '\0') {
        while (*ptr != '\0' && *ptr == ' ') {
            ptr++;
        }
        if (*ptr == '\0') {
            break;
        }
        words[wordCount].begin = ptr;

        while (*ptr != '\0' && *ptr != ' ') {
            ptr++;
        }

        words[wordCount].end = ptr;
        wordCount++;

        while (*ptr != '\0' && *ptr == ' ') {
            ptr++;
        }
    }

    for (int i = 1; i < wordCount; i++) {
        char *ptr1 = words[i - 1].begin;
        char *ptr2 = words[i].begin;

        while (ptr1 != words[i - 1].end && ptr2 != words[i].end) {
            if (*ptr1 != *ptr2) {
                if (*ptr1 < *ptr2) {
                    break;
                } else {
                    return 0;
                }
            }
            ptr1++;
            ptr2++;
        }

        if (ptr1 == words[i - 1].end && ptr2 != words[i].end) {
            return 0;
        }
    }

    return 1;
}

void getBagOfWords(BagOfWords *bag, char *s) {
    bag->size = 0;

    char *ptr = s;
    while (*ptr != '\0') {
        while (*ptr != '\0' && *ptr == ' ') {
            ptr++;
        }
        if (*ptr == '\0') {
            break;
        }

        bag->words[bag->size].begin = ptr;

        while (*ptr != '\0' && *ptr != ' ') {
            ptr++;
        }

        bag->words[bag->size].end = ptr;
        bag->size++;

        while (*ptr != '\0' && *ptr == ' ') {
            ptr++;
        }
    }
}

void printReverseBagOfWords(char *s) {
    BagOfWords bag;

    getBagOfWords(&bag, s);

    for (int i = bag.size - 1; i >= 0; i--) {
        char *ptr = bag.words[i].begin;
        while (ptr != bag.words[i].end) {
            printf("%c", *ptr);
            ptr++;
        }
        printf("\n");
    }
}

int isPalindrome(char *s, size_t length) {
    char *begin = s;
    char *end = s + length - 1;

    while (begin < end) {
        if (*begin != *end) {
            return 0;
        }
        begin++;
        end--;
    }

    return 1;
}

int getCountOfPalindrome(char *s) {
    int count = 0;

    char *ptr = s;
    char *wordStart = ptr;

    while (*ptr != '\0') {
        if (*ptr == ',' || *ptr == ' ') {
            if (isPalindrome(wordStart, ptr - wordStart)) {
                count++;
            }
            wordStart = ptr + 1;
        }
        ptr++;
    }

    if (isPalindrome(wordStart, ptr - wordStart)) {
        count++;
    }

    return count;
}


int getWord(char **ptr, WordDescriptor *word) {
    char *begin = *ptr;
    while (**ptr != '\0' && **ptr == ' ') {
        (*ptr)++;
    }

    word->begin = *ptr;

    while (**ptr != '\0' && **ptr != ' ') {
        (*ptr)++;
    }

    word->end = *ptr;

    while (**ptr != '\0' && **ptr == ' ') {
        (*ptr)++;
    }

    return *ptr != begin;
}

void interleaveStrings(char *s1, char *s2, char *result) {
    WordDescriptor word1, word2;
    int isW1Found, isW2Found;
    char *beginSearch1 = s1, *beginSearch2 = s2;
    char *resultPtr = result;

    while ((isW1Found = getWord(&beginSearch1, &word1)),
            (isW2Found = getWord(&beginSearch2, &word2)),
            isW1Found || isW2Found) {

        if (isW1Found) {
            char *ptr = word1.begin;
            while (ptr != word1.end) {
                *resultPtr = *ptr;
                resultPtr++;
                ptr++;
            }
            *resultPtr = ' ';
            resultPtr++;
        }

        if (isW2Found) {
            char *ptr = word2.begin;
            while (ptr != word2.end) {
                *resultPtr = *ptr;
                resultPtr++;
                ptr++;
            }
            *resultPtr = ' ';
            resultPtr++;
        }
    }

    *resultPtr = '\0';
}

void reverseWords(char *s) {
    BagOfWords bag;

    getBagOfWords(&bag, s);

    for (int i = bag.size - 1; i >= 0; i--) {
        char *ptr = bag.words[i].begin;
        while (ptr != bag.words[i].end) {
            printf("%c", *ptr);
            ptr++;
        }
        printf(" ");
    }
}

WordBeforeFirstWordWithAReturnCode getWordBeforeFirstWordWithA(char *s, WordDescriptor *w) {
    WordDescriptor word;
    char *ptr = s;
    char *prevWordEnd = NULL;
    int foundAWord = 0;

    while (getWord(&ptr, &word)) {
        if (find(word.begin, word.end, 'a') != word.end) {
            if (!foundAWord) {
                *w = word;
                return FIRST_WORD_WITH_A;
            }
            else {
                if (prevWordEnd != NULL) {
                    w->begin = s;
                    w->end = prevWordEnd;
                    return WORD_FOUND;
                } else {
                    return EMPTY_STRING;
                }
            }
        }

        prevWordEnd = word.end;
        foundAWord = 1;
    }

    if (ptr == s) {
        return EMPTY_STRING;
    }

    return NOT_FOUND_A_WORD_WITH_A;
}

WordDescriptor lastWordInFirstStringInSecondString(char *s1, char *s2) {
    BagOfWords bag1, bag2;
    getBagOfWords(&bag1, s1);
    getBagOfWords(&bag2, s2);

    for (int i = bag1.size - 1; i >= 0; i--) {
        WordDescriptor word = bag1.words[i];
        for (int j = 0; j < bag2.size; j++) {
            if (areWordsEqual(word, bag2.words[j])) {
                return word;
            }
        }
    }

    WordDescriptor emptyWord = {"", ""};
    return emptyWord;
}

void wordDescriptorToString(WordDescriptor word, char *destination) {
    char *ptr = destination;
    char *wordPtr = word.begin;
    while (wordPtr != word.end) {
        *ptr++ = *wordPtr++;
    }
    *ptr = '\0';
}

bool hasDuplicateWords(char *s) {
    BagOfWords bag;
    getBagOfWords(&bag, s);

    for (int i = 0; i < bag.size - 1; i++) {
        for (int j = i + 1; j < bag.size; j++) {
            if (areWordsEqual(bag.words[i], bag.words[j])) {
                return true;
            }
        }
    }

    return false;
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

void test_areWordsLexicographicallyOrdered() {
    char s1[] = "apple banana cherry";
    char s2[] = "test false apple";

    assert(areWordsLexicographicallyOrdered(s1) == 1 && areWordsLexicographicallyOrdered(s2) == 0);
}

void test_getCountOfPalindrome() {
    char s1[] = "radar level apple skin";
    char s2[] = "test false apple skin";
    char s3[] = "radar level lol anna";

    assert(getCountOfPalindrome(s1) == 2 && getCountOfPalindrome(s2) == 0 && getCountOfPalindrome(s3) == 4);
}


void test_interleaveStrings() {
    char s1[] = "test test2 test3";
    char s2[] = "one two three four five";
    char result[MAX_STRING_SIZE];

    interleaveStrings(s1, s2, result);

    ASSERT_STRING("test one test2 two test3 three four five ", result);
}

void test_getWordBeforeFirstWordWithA() {
    WordDescriptor word;
    char s1[] = "apple banana cherry";
    char s2[] = "test cherry";
    char s3[] = "";
    char s4[] = "test apple";

    assert(getWordBeforeFirstWordWithA(s1, &word) == FIRST_WORD_WITH_A && getWordBeforeFirstWordWithA(s2, &word) == NOT_FOUND_A_WORD_WITH_A &&
           getWordBeforeFirstWordWithA(s3, &word) == EMPTY_STRING && getWordBeforeFirstWordWithA(s4, &word) == WORD_FOUND);

}

void test_lastWordInFirstStringInSecondString() {
    char s1[] = "apple banana cherry";
    char s2[] = "banana cherry";
    char s3[] = "banana";
    char s4[] = "pineapple orange";

    char result[MAX_WORD_SIZE];
    WordDescriptor word;

    word = lastWordInFirstStringInSecondString(s1, s2);
    wordDescriptorToString(word, result);
    ASSERT_STRING("cherry", result);

    word = lastWordInFirstStringInSecondString(s1, s3);
    wordDescriptorToString(word, result);
    ASSERT_STRING("banana", result);

    word = lastWordInFirstStringInSecondString(s1, s4);
    wordDescriptorToString(word, result);
    ASSERT_STRING("", result);
}

void test_hasDuplicateWords() {
    char s1[] = "hello world hello";
    char s2[] = "hello world";
    assert(hasDuplicateWords(s1) == true && hasDuplicateWords(s2) == false);
}

int main() {
    test_removeNonLetters();
    test_removeExtraSpaces();
    test_replaceDigitsWithSpaces();
    test_replace();
    test_areWordsLexicographicallyOrdered();
    test_getCountOfPalindrome();
    test_interleaveStrings();
    test_getWordBeforeFirstWordWithA();
    test_lastWordInFirstStringInSecondString();
    test_hasDuplicateWords();

    return 0;
}