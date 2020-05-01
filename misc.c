//
// Created by ilya on 30.04.2020.
//

#include "lol.h"

char* subString (const char * input, int offset, int len, char * dest) {
    if(offset < -9){
        offset = 0;
        len = 0;
    }
    if(offset < 0){
        len = len + offset;
        offset = 0;
    }
    int input_len = (int) strlen (input);
    if (offset + len > input_len)
    {
        return NULL;
    }
    strncpy (dest, input + offset, len);
    return dest;
}

void setCharNulls(char * str, int len){
    for (int i = 0; i < len; i++){
        str[i] = '\0';
    }
}

void setIntNulls(int * str, int len){
    for (int i = 0; i < len; i++){
        str[i] = '\0';
    }
}

void printBigNum(const int * num, int len){
    int start = len - 1;
    while(1){
        if(num[start] != 0){
            break;
        }
        start--;
        if(start == -1){
            fprintf(stdout, "0");
            return;
        }
    }
    fprintf(stdout, "%d", num[start]);
    start--;
    for(int i = start; i >= 0; i--){
        fprintf(stdout, "%09d", num[i]);
    }
    fprintf(stdout, "\n");
    fflush(stdout);
}

int sumBlocks(const int * block1, const int * block2, int * res, int len, int add){
    int loc_add = add;
    for (int i = 0; i < len; i++) {
        res[i] = block1[i] + block2[i] + loc_add;
        if(res[i] > 999999999){
            res[i] = res[i] - 1000000000;
            loc_add = 1;
        } else {
            loc_add = 0;
        }
    }
    return loc_add;
}

void writeBigNum(int * num, int len, FILE * file){
    int start = len - 1;
    while(1){
        if(num[start] != 0){
            break;
        }
        start--;
        if(start == -1){
            fprintf(file, "0");
            return;
        }
    }
    fprintf(file, "%d", num[start]);
    start--;
    for(int i = start; i >= 0; i--){
        fprintf(file, "%09d", num[i]);
    }
    fprintf(file, "\n");
}

int add_pre_result(pre_res * pre_results, const int params[4]){
    int place = params[3];
    pre_results[place].place = place;
    pre_results[place].add1 = params[0];
    pre_results[place].add2 = params[1];
    pre_results[place].len = params[2];
    return place;
}
