#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "sort.cpp"

struct Text{
    char* textPtr;
    size_t sizeTextBuffer;
    size_t realSizeText;
    size_t numLines;
    char** arrOfStrPtr;
};

size_t getfileSize(const char* fileName);
void setTextCharacter(Text* text, size_t fileSize, int fileDesc);

size_t countNumLines(char* text, size_t realSizeText);
void convertTextIntoArrOfStrPtr(char* text, char** arrOfStrPtr, size_t realSizeText, size_t numLines);

void printLines(char** arrOfStrPtr, size_t numLines);
void printLinesInFile(FILE* filePtr, char** arrOfStrPtr, size_t numLines);

bool isLetter(char c);
bool isEmptyLine(const char* str);
bool doesStringContainLetter(const char* str, size_t length);

int wordsComparatorUp(const void* ptrstr1, const void* ptrstr2);
int wordsComparatorUpReverse(const void* ptrstr1, const void* ptrstr2);
int comparatorPtrOfStrUp(const void* a, const void* b);
int strCmpWithoutPunctuationAndRegister(const char* str1, const char* str2);
int strCmpWithoutPunctuationAndRegisterReverse(const char* str1, const char* str2);

int main(){
    const char* fileName = "Eugene_Onegin.txt";
    assert(fileName != NULL);

    size_t fileSize = getfileSize(fileName);
    int fileDesc = open(fileName, O_RDONLY);
    assert(fileDesc >= 0);

    Text text = {}; 
    setTextCharacter(&text, fileSize, fileDesc);
    assert(text.arrOfStrPtr != NULL);

    FILE* filePtr = fopen("oneginSort.txt", "w");
    assert(filePtr != NULL);

    qSort(text.arrOfStrPtr, text.numLines, sizeof(text.arrOfStrPtr[0]), wordsComparatorUp);
    printLinesInFile(filePtr, text.arrOfStrPtr, text.numLines);

    qsort(text.arrOfStrPtr, text.numLines, sizeof(text.arrOfStrPtr[0]), wordsComparatorUpReverse);
    printLinesInFile(filePtr, text.arrOfStrPtr, text.numLines);

    qsort(text.arrOfStrPtr, text.numLines, sizeof(text.arrOfStrPtr[0]), comparatorPtrOfStrUp);
    printLinesInFile(filePtr, text.arrOfStrPtr, text.numLines);

    free(text.textPtr);
    free(text.arrOfStrPtr);

    close(fileDesc);
    fclose(filePtr);

    return 0;
}

size_t countNumLines(char* text, size_t realSizeText){
    size_t numLines = 1;
    size_t i = 0;
    while(i < realSizeText){
        assert(i < realSizeText);
        if(text[i] == '\n'){
           text[i] =  '\0';
            //printf("num of line = %d, count symbol in string = %d\n", numLines, i);
            numLines++;
        }
        i++;
    }
    //printf("final num of lines = %d, final count symbol in text = %d\n", numLines, i);
    return numLines;
}

void convertTextIntoArrOfStrPtr(char* text, char** arrOfStrPtr, size_t realSizeText, size_t numLines){
    assert(arrOfStrPtr != NULL);
    assert(text != NULL);

    arrOfStrPtr[0] = text;
    size_t indexOfStr = 1;
    size_t i = 0;
    while(i < realSizeText && indexOfStr < numLines){
        assert(i < realSizeText);
        assert(indexOfStr < numLines);
        if(text[i] == '\0'){
            //printf("\nplus one slash 0, i = %d\n", i);
            arrOfStrPtr[indexOfStr] = &text[i + 1];
            assert(arrOfStrPtr[indexOfStr] != NULL); 
            //printf("%c\n", arrOfStrPtr[indexOfStr][0]);
            indexOfStr++;
        }
        i++;
    }
    
    text[i] = '\0';
    //printf("\nThats all))\n");
    //printf("\nindex = <%d>\n", indexOfStr);
}

void printLines(char** arrOfStrPtr, size_t numLines){
    assert(arrOfStrPtr != NULL);
    for(size_t i = 0; i < numLines; i++){
        //printf("\n%d\n", i);
        assert(i < numLines);
        assert(arrOfStrPtr[i] != NULL);
        size_t lengthOfElemArrOfStrPtr = strlen(arrOfStrPtr[i]);
        //printf("%d\n", lengthOfElemArrOfStrPtr);
        //printf("\n%d\n", lengthOfElemArrOfStrPtr);
        if(!isEmptyLine(arrOfStrPtr[i])){
            for(size_t j = 0; j < lengthOfElemArrOfStrPtr; j++){
                assert(j < strlen(arrOfStrPtr[i]));
                printf("%c", arrOfStrPtr[i][j]);
            }
            printf("\n");
        }
    }
}

void printLinesInFile(FILE* filePtr, char** arrOfStrPtr, size_t numLines){
    assert(filePtr != NULL);
    assert(arrOfStrPtr != NULL);

    for(size_t i = 0; i < numLines; i++){
        //printf("\n%d\n", i);
        assert(arrOfStrPtr[i] != NULL);
        assert(i < numLines);
        size_t lengthOfElemArrOfStrPtr = strlen(arrOfStrPtr[i]);
        //printf("%d\n", lengthOfElemArrOfStrPtr);
        //printf("\n%d\n", lengthOfElemArrOfStrPtr);
        if(!isEmptyLine(arrOfStrPtr[i])){
            for(size_t j = 0; j < lengthOfElemArrOfStrPtr; j++){
                assert(j < strlen(arrOfStrPtr[i]));
                fprintf(filePtr, "%c", arrOfStrPtr[i][j]);
            }
            fprintf(filePtr, "\n");
        }
    }
    fprintf(filePtr,"---------------------------------------------------------------------\n\n\n");
}

int wordsComparatorUp(const void* ptrstr1, const void* ptrstr2){
    assert(ptrstr1 != NULL);
    assert(ptrstr2 != NULL);
    
    const char* str1 = *((const char* const*) ptrstr1);
    const char* str2 = *((const char* const*) ptrstr2);
    
    assert(str1 != NULL);
    assert(str2 != NULL);

    int res = strCmpWithoutPunctuationAndRegister(str1, str2);
    return res;
}

int wordsComparatorUpReverse(const void* ptrstr1, const void* ptrstr2){
    assert(ptrstr1 != NULL);
    assert(ptrstr2 != NULL);

    const char* str1 = *((const char* const*) ptrstr1);
    const char* str2 = *((const char* const*) ptrstr2);

    assert(str1 != NULL);
    assert(str2 != NULL);

    int res = strCmpWithoutPunctuationAndRegisterReverse(str1, str2);
    return res;
}

bool isLetter(char c){
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
        return true;
    }
    return false;
}

bool isEmptyLine(const char* str){
    assert(str != NULL);

    size_t i = 0;
    while(i < strlen(str)){
        assert(i < strlen(str));
        if(str[i] != ' '){
            return false;
        }
        i++;
    } 
    return true;
}

int comparatorPtrOfStrUp(const void* a, const void* b){
    assert(a != NULL);
    assert(b != NULL);

    const char* ptr1 = *((const char* const*) a);
    const char* ptr2 = *((const char* const*) b);

    assert(ptr1 != NULL);
    assert(ptr2 != NULL);

    return (ptr1 - ptr2);
}

size_t getfileSize(const char* fileName){
    assert(fileName != NULL);

    struct stat fileInfo = {};
    stat(fileName, &fileInfo);
    return fileInfo.st_size;
}

void setTextCharacter(Text* text, size_t fileSize, int fileDesc){
    assert(text != NULL);
    text->sizeTextBuffer = fileSize + 1;

    text->textPtr = (char*) calloc(text->sizeTextBuffer, sizeof(char));
    assert(text->textPtr != NULL);

    text->realSizeText = read(fileDesc, (void*) text->textPtr, fileSize) + 1;
    text->numLines = countNumLines(text->textPtr, text->realSizeText) + 1;
    
    text->arrOfStrPtr = (char**) calloc(text->numLines, sizeof(char*));
    assert(text->arrOfStrPtr != NULL);

    convertTextIntoArrOfStrPtr(text->textPtr, text->arrOfStrPtr, text->realSizeText, text->numLines);
}

int strCmpWithoutPunctuationAndRegister(const char* str1, const char* str2){
    assert(str1 != NULL);
    assert(str2 != NULL);

    size_t i1 = 0;
    size_t i2 = 0;
    size_t size1 = strlen(str1);
    size_t size2 = strlen(str2);

    while(i1 < size1 && i2 < size2){
        assert(i1 < size1);
        assert(i2 < size2);

        if(isLetter(str1[i1]) && isLetter(str2[i2])){
            char c1 = (char) tolower(str1[i1]);
            char c2 = (char) tolower(str2[i2]);
            if(c1 > c2){
                return 1;
            }
            else if(c2 > c1){
                return -1;
            }
            i1++;
            i2++;
        }
        else if(isLetter(str1[i1]) && !isLetter(str2[i2])){
            i2++;
        }
        else if(!isLetter(str1[i1]) && isLetter(str2[i2])){
            i1++;
        }
        else{
            i1++;
            i2++;
        }
    }

    assert(i1 <= size1);
    assert(i2 <= size2);

    if(i1 == size1){
        assert(str2 != NULL);
        assert(size2 >= i2);
        if(doesStringContainLetter((const char*) str2 + i2, size2 - i2)){
            return -1;
        }
    }
    if(i2 == size2){
        assert(str1 != NULL);
        assert(size1 >= i1);
        if(doesStringContainLetter((const char*) str1 + i1, size1 - i1)){
            return 1;
        }
    }
    return 0;
}

int strCmpWithoutPunctuationAndRegisterReverse(const char* str1, const char* str2){
    assert(str1 != NULL);
    assert(str2 != NULL);

    size_t size1 = strlen(str1);
    size_t size2 = strlen(str2);    

    int i1 = size1 - 1;
    int i2 = size2 - 1;

    while(i1 >= 0 && i2 >= 0){
        assert(i1 >= 0);
        assert(i2 >= 0);
        if(isLetter(str1[i1]) && isLetter(str2[i2])){
            char c1 = (char) tolower(str1[i1]);
            char c2 = (char) tolower(str2[i2]);
            if(c1 > c2){
                return 1;
            }
            else if(c2 > c1){
                return -1;
            }
            i1--;
            i2--;
        }
        else if(isLetter(str1[i1]) && !isLetter(str2[i2])){
            i2--;
        }
        else if(!isLetter(str1[i1]) && isLetter(str2[i2])){
            i1--;
        }
        else{
            i1--;
            i2--;
        }
    }

    if(i2 >= 0){
        assert(str2 != NULL);
        if(doesStringContainLetter(str2, i2 + 1)){
            return -1;
        }
    }
    else if(i1 >= 0){
        assert(str1 != NULL);
        if(doesStringContainLetter(str1, i1 + 1)){
            return 1;
        }
    }
    return 0;
}

bool doesStringContainLetter(const char* str, size_t length){
    assert(str != NULL);

    size_t i = 0;
    while(i < length){
        assert(i < length);
        if(isLetter(str[i])){
            return true;
        }
        i++;
    }
    return false;
}

