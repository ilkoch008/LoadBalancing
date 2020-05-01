//
// Created by ilya on 30.04.2020.
//

#ifndef SUMOFBIGNUMBERS_LOL_H
#define SUMOFBIGNUMBERS_LOL_H

#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>

#define STATIC 11
#define DYNAMIC 22
#define READY 33
#define FINISHED 44

struct pre_res {
    int add1;     // params[0]
    int add2;     // params[1]
    int * result1;
    int * result2;
    int len;      // params[2]
    int place;    // params[3]
} typedef pre_res;

char* subString (const char * input, int offset, int len, char * dest);
void setCharNulls(char * str, int len);
void setIntNulls(int * str, int len);
void printBigNum(const int * num, int len);
void writeBigNum(int * num, int len, FILE * file);

void calculateStaticNotRoot();
int * calculateStaticRoot(int * num1, int * num2, int len);
void calculateDynamicNotRoot();
int * calculateDynamicRoot(int * num1, int * num2, int len, int size_of_block);

int sumBlocks(const int * block1, const int * block2, int * res, int len, int add);

int add_pre_result(pre_res * pre_results, const int params[4]);

#endif //SUMOFBIGNUMBERS_LOL_H
