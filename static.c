//
// Created by ilya on 30.04.2020.
//

#include "lol.h"

void calculateStaticNotRoot(){
    int size = 0;
    int numOfProcs, myRank;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int size_of_my_block;
    MPI_Recv(&size_of_my_block, 1, MPI_INT, 0, STATIC, MPI_COMM_WORLD, &status);
    int * my_block1 = malloc(size_of_my_block * sizeof(int));
    int * my_block2 = malloc(size_of_my_block * sizeof(int));
    MPI_Recv(my_block1, size_of_my_block, MPI_INT, 0, STATIC, MPI_COMM_WORLD, &status);
    MPI_Recv(my_block2, size_of_my_block, MPI_INT, 0, STATIC, MPI_COMM_WORLD, &status);
    int * result1 = malloc(size_of_my_block * sizeof(int));
    int * result2 = NULL;
    int oops = ((my_block1[0] + my_block2[0]) == 999999999);
    int add1;
    int add2;
    int add_from_side;
    add1 = sumBlocks(my_block1, my_block2, result1, size_of_my_block, 0);
    if(numOfProcs > 2) {
        if (oops && myRank != 1) {
            result2 = malloc(size_of_my_block * sizeof(int));
            add2 = sumBlocks(my_block1, my_block2, result2, size_of_my_block, 1);
        }
        if (myRank == 1) {
            MPI_Send(&add1, 1, MPI_INT, 2, STATIC, MPI_COMM_WORLD);
            MPI_Send(result1, size_of_my_block, MPI_INT, 0, STATIC, MPI_COMM_WORLD);
        } else if (myRank == numOfProcs - 1) {
            MPI_Recv(&add_from_side, 1, MPI_INT, numOfProcs - 2, STATIC, MPI_COMM_WORLD, &status);
            if (oops && add_from_side != 0) {
                MPI_Send(result2, size_of_my_block, MPI_INT, 0, STATIC, MPI_COMM_WORLD);
            } else {
                result1[0] += add_from_side;
                MPI_Send(result1, size_of_my_block, MPI_INT, 0, STATIC, MPI_COMM_WORLD);
            }
        } else {
            MPI_Recv(&add_from_side, 1, MPI_INT, myRank - 1, STATIC, MPI_COMM_WORLD, &status);
            if (oops && add_from_side != 0) {
                MPI_Send(&add2, 1, MPI_INT, myRank + 1, STATIC, MPI_COMM_WORLD);
                MPI_Send(result2, size_of_my_block, MPI_INT, 0, STATIC, MPI_COMM_WORLD);
            } else {
                MPI_Send(&add1, 1, MPI_INT, myRank + 1, STATIC, MPI_COMM_WORLD);
                result1[0] += add_from_side;
                MPI_Send(result1, size_of_my_block, MPI_INT, 0, STATIC, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Send(result1, size_of_my_block, MPI_INT, 0, STATIC, MPI_COMM_WORLD);
    }

    free(result1);
    if(result2 != NULL){
        free(result2);
    }
    free(my_block1);
    free(my_block2);
}
int * calculateStaticRoot(int * num1, int * num2, int len){
    int size = 0;
    int numOfProcs, myRank;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int numOfSlaves = numOfProcs - 1;
    int * blocks = malloc(numOfSlaves * sizeof(int));
    int remainder = len % numOfSlaves;
    int avg_block = len / numOfSlaves;
    for(int i = numOfSlaves-1; i >= 0; i--){
        blocks[i] = avg_block;
        if(remainder > 0){
            blocks[i]++;
            remainder--;
        }
    }
    int point = 0;
    for(int i = 0; i < numOfSlaves; i++){
        MPI_Send(&blocks[i], 1, MPI_INT, i+1, STATIC, MPI_COMM_WORLD);
    }
    for(int i = 0; i < numOfSlaves; i++){
        MPI_Send(&num1[point], blocks[i], MPI_INT, i+1, STATIC, MPI_COMM_WORLD);
        MPI_Send(&num2[point], blocks[i], MPI_INT, i+1, STATIC, MPI_COMM_WORLD);
        point += blocks[i];
    }

    int * res = malloc(len * sizeof(int));

    point = 0;
    for(int i = 0; i < numOfSlaves; i++){
        MPI_Recv(&res[point], blocks[i], MPI_INT, i+1, STATIC, MPI_COMM_WORLD, &status);
        point += blocks[i];
    }

    free(blocks);
    return res;
}
