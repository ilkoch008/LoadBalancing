//
// Created by ilya on 30.04.2020.
//

#include "lol.h"

void calculateDynamicNotRoot(){
    int size = 0;
    int numOfProcs, myRank;
    int ready = READY;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int size_of_my_block;
    int place;
    while(1) {
        MPI_Send(&ready, 1, MPI_INT, 0, READY, MPI_COMM_WORLD);                         //===================
        MPI_Recv(&size_of_my_block, 1, MPI_INT, 0, DYNAMIC, MPI_COMM_WORLD, &status); // Getting task
        if(size_of_my_block == 0){
            break;
        }
        MPI_Recv(&place, 1, MPI_INT, 0, DYNAMIC, MPI_COMM_WORLD, &status);
        int *my_block1 = malloc(size_of_my_block * sizeof(int));
        int *my_block2 = malloc(size_of_my_block * sizeof(int));
        MPI_Recv(my_block1, size_of_my_block, MPI_INT, 0, DYNAMIC, MPI_COMM_WORLD, &status); // Getting task
        MPI_Recv(my_block2, size_of_my_block, MPI_INT, 0, DYNAMIC, MPI_COMM_WORLD, &status); //==================
 //       if(myRank == 2) {
//            printf("myRank = %d; size_of_my_block = %d; my blocks:\n", myRank, size_of_my_block);
//            printBigNum(my_block1, size_of_my_block);
//            printBigNum(my_block2, size_of_my_block);
  //      }

        int *result1 = malloc(size_of_my_block * sizeof(int));
        int *result2 = NULL;
        int oops = ((my_block1[0] + my_block2[0]) == 999999999);
        int add1 = 0;
        int add2 = 0;
        int params[4];
        add1 = sumBlocks(my_block1, my_block2, result1, size_of_my_block, 0);
        if (oops) {
            result2 = malloc(size_of_my_block * sizeof(int));
            add2 = sumBlocks(my_block1, my_block2, result2, size_of_my_block, 1);
        }
        ready = FINISHED;
        MPI_Send(&ready, 1, MPI_INT, 0, READY, MPI_COMM_WORLD);                 //==========================
        MPI_Send(&oops, 1, MPI_INT, 0, DYNAMIC, MPI_COMM_WORLD);                // Sending results
        params[0] = add1;
        params[1] = add2;
        params[2] = size_of_my_block;
        params[3] = place;
        MPI_Send(params, 4, MPI_INT, 0, DYNAMIC, MPI_COMM_WORLD);
        MPI_Send(result1, size_of_my_block, MPI_INT, 0, DYNAMIC, MPI_COMM_WORLD);
        if (oops) {
            MPI_Send(result2, size_of_my_block, MPI_INT, 0, DYNAMIC, MPI_COMM_WORLD); // Sending results
        }                                                                                  //==========================
        free(result1);
        if (result2 != NULL) {
            free(result2);
        }
        free(my_block1);
        free(my_block2);
        ready = READY;
    }
}
int * calculateDynamicRoot(int * num1, int * num2, int len, int size_of_block){
    int size = 0;
    int numOfProcs, myRank;
    int ready;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int position = 0;
    int place = 0;
    int oops;
    int params[4];
    int current_place;
    int numOfSlaves = numOfProcs - 1;

    int size_of_pre_results = len / size_of_block;
    if(len % size_of_block != 0){
        size_of_pre_results++;
    }
    pre_res * pre_results = malloc(size_of_pre_results * sizeof(pre_res));
    int first_time = 1;
    int size_of_last_block;
    int finished_processes_counter = 0;
    while(1){
        setIntNulls(params, 4);
        int source;
        MPI_Recv(&ready, 1, MPI_INT, MPI_ANY_SOURCE, READY, MPI_COMM_WORLD, &status);
        source = status.MPI_SOURCE;
        if(ready == READY && position + size_of_block < len) { // Giving a task
            MPI_Send(&size_of_block, 1, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD);
            MPI_Send(&place, 1, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD);
            MPI_Send(&num1[position], size_of_block, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD);
            MPI_Send(&num2[position], size_of_block, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD);
            position += size_of_block;
            place++;
        } else if(ready == READY && position + size_of_block >= len){
            if(first_time){
                size_of_last_block = len - position;
                MPI_Send(&size_of_last_block, 1, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD);
                MPI_Send(&place, 1, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD);
                MPI_Send(&num1[position], size_of_last_block, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD);
                MPI_Send(&num2[position], size_of_last_block, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD);
                position += size_of_last_block;
                place++;
                first_time = 0;
            } else {
                int zero = 0;
                MPI_Send(&zero, 1, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD);
                finished_processes_counter++;
                if(finished_processes_counter == numOfSlaves){
                    break;
                }
            }
        } else if(ready == FINISHED) { // Getting a result
            MPI_Recv(&oops, 1, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD, &status);
            int *result1 = malloc(size_of_block * sizeof(int));
            int *result2 = NULL;
            MPI_Recv(params, 4, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD, &status);
            current_place = add_pre_result(pre_results, params);
            MPI_Recv(result1, size_of_block, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD, &status);
            if(oops){
                result2 = malloc(size_of_block * sizeof(int));
                MPI_Recv(result2, size_of_block, MPI_INT, source, DYNAMIC, MPI_COMM_WORLD, &status);
            }
            pre_results[current_place].result1 = result1;
            pre_results[current_place].result2 = result2;
        }
    }

    int * res = malloc(len * sizeof(int));
    int add_previous = 0;
    int pre_res_ints_len;
    int k = 0;

    for(int i = 0; i < size_of_pre_results; i++){
        pre_res_ints_len = pre_results[i].len;
        if(add_previous == 0) {
            add_previous = pre_results[i].add1;
            for (int j = 0; j < pre_res_ints_len; j++) {
                res[k+j] = pre_results[i].result1[j];
            }
        } else if(pre_results[i].result2 == NULL){
            pre_results[i].result1[0] += 1;
            for (int j = 0; j < pre_res_ints_len; j++) {
                res[k+j] = pre_results[i].result1[j];
            }
            add_previous = pre_results[i].add1;
        } else {
            for (int j = 0; j < pre_res_ints_len; j++) {
                res[k+j] = pre_results[i].result2[j];
            }
            add_previous = pre_results[i].add2;
        }
        k += pre_results[i].len;
    }

    for(int i = 0; i < size_of_pre_results; i++){
        free(pre_results[i].result1);
        if(pre_results[i].result2 != NULL){
            free(pre_results[i].result2);
        }
    }
    free(pre_results);
    return res;
}
