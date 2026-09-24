int reconfiguration(void* arr, size_t size, size_t sizeOfElem, int (*comparator) (const void* a, const void* b));
void swap(void* a, void* b, size_t sizeOfElem);
void qSort(void* arr, size_t size, size_t sizeOfElem, int (*comparator) (const void* a, const void* b));
 
int reconfiguration(void* arr, size_t size, size_t sizeOfElem, int (*comparator) (const void* a, const void* b)){
    size_t middleInd = size / 2;
    size_t left = 0;
    size_t right = size - 1;

    assert(arr != NULL);
    assert(*(char**) arr != NULL);

    while (true) {

        while(comparator((char*)arr + left * sizeOfElem, (char*)arr + middleInd * sizeOfElem) <= 0
                                                             && left < right && left < middleInd){
            left++;
        }
        while(comparator((char*)arr + right * sizeOfElem, (char*)arr + middleInd * sizeOfElem) > 0 
                                                             && left < right && right > middleInd){
            right--;
        }

        //printf("left = %d\n", left);
        //printf("right = %d\n", right);
        //printf("left = %c\n", **(char**)((char*)arr + left * sizeOfElem));
        //printf("right = %c\n", **(char**)((char*)arr + right * sizeOfElem));
        //getchar();

        if(left >= right){
            break;
        }
        if(right == middleInd){
            middleInd = left;
        }
        else if(left == middleInd){
            middleInd = right;
        }
        swap((void*) ((char*)arr + left * sizeOfElem), (void*) ((char*)arr + right * sizeOfElem), sizeOfElem);
    }
    //printf("end of reconf\n");
    return middleInd;
}

void swap(void* a, void* b, size_t sizeOfElem){
    char temp = '\0';
    for(size_t i  = 0; i < sizeOfElem; i++){
        temp = *((char*) a + i);
        *((char*) a + i) = *((char*) b + i);
        *((char*) b + i) = temp;
    }
}


void qSort(void* arr, size_t size, size_t sizeOfElem, int (*comparator) (const void* a, const void* b)){
    size_t newMiddleInd  = reconfiguration(arr, size, sizeOfElem, *comparator);
    if(size > 1){
        if(newMiddleInd != 0){
            qSort((char*) arr, newMiddleInd, sizeOfElem, comparator);
        }
        if(size - newMiddleInd - 1 != 0){
            qSort((char*) arr + (newMiddleInd + 1) * sizeOfElem, size - newMiddleInd - 1, sizeOfElem, comparator);
        }
    }
}





