#include "lol.h"

int main(int argc, char **argv) {

    int size = 0;
    MPI_Init(&argc, &argv);
    int numOfProcs, myRank;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if(numOfProcs < 2){
        fprintf(stderr, "Number of processes must be more than one\n");
        exit(1);
    }

    char *input = argv[1];
    char *output = argv[2];
    char *mode_str = argv[3];
    char *size_of_block_str = NULL;

    if (myRank == 0) {
        if (argc == 4) {
            input = argv[1];
            output = argv[2];
            mode_str = argv[3];
        } else if (argc == 5) {
            input = argv[1];
            output = argv[2];
            mode_str = argv[3];
            size_of_block_str = argv[4];
        } else {
            fprintf(stderr, "There should be 3 or 4 parameters\n");
            exit(1);
        }
    }

    int mode;

    if (myRank == 0) {
        FILE *input_file;
        FILE *output_file;
        int size_of_block;
        int size_of_nums = 0;
        input_file = fopen(input, "r");
        output_file = fopen(output, "w");

        if (strstr(mode_str, "dynamic")) {
            mode = DYNAMIC;
        } else if (strstr(mode_str, "static")) {
            mode = STATIC;
        } else {
            fprintf(stderr, "Mode should be dynamic or static\n");
            exit(1);
        }
        char *endptr;
        if (mode == DYNAMIC) {
            if (size_of_block_str != NULL) {
                endptr = strstr(size_of_block_str, "\0");
                size_of_block = (int) strtol(size_of_block_str, &endptr, 10);
            } else {
                size_of_block = 1;
            }
        }

        fscanf(input_file, "%d", &size_of_nums);

        if (size_of_nums == 0) {
            fprintf(stderr, "Wrong format of input file\n");
            exit(1);
        }
        char *number1_str = malloc(size_of_nums * sizeof(char));
        char *number2_str = malloc(size_of_nums * sizeof(char));
        fscanf(input_file, "%s", number1_str);
        fscanf(input_file, "%s", number2_str);
        int num1_len = (int) strlen(number1_str);
        int num2_len = (int) strlen(number2_str);
        int num_len = (num1_len > num2_len) ? num1_len : num2_len;


        int num_of_ints = (int) num_len / 9;
        num_of_ints++;

        int *num1 = calloc(num_of_ints, sizeof(int));
        int *num2 = calloc(num_of_ints, sizeof(int));
        char *inter = malloc(10 * sizeof(char));

        for (int i = 0; i < num_of_ints; i++) {
            setCharNulls(inter, 10);
            subString(number1_str, num1_len - (i + 1) * 9, 9, inter);
            num1[i] = (int) strtol(inter, NULL, 10);
        }
        for (int i = 0; i < num_of_ints; i++) {
            setCharNulls(inter, 10);
            subString(number2_str, num2_len - (i + 1) * 9, 9, inter);
            num2[i] = (int) strtol(inter, NULL, 10);
        }

        MPI_Bcast(&mode, 1, MPI_INT, 0, MPI_COMM_WORLD);

        int *result;
        double dtime;
        if(myRank == 0){
            dtime = MPI_Wtime();
        }
        if (mode == STATIC) {
            result = calculateStaticRoot(num1, num2, num_of_ints);
            dtime = MPI_Wtime() - dtime;
            printf("time of computing = %f\n", dtime);
            writeBigNum(result, num_of_ints, output_file);
        }
        if (mode == DYNAMIC) {
            result = calculateDynamicRoot(num1, num2, num_of_ints, size_of_block);
            dtime = MPI_Wtime() - dtime;
            printf("time of computing = %f\n", dtime);
            writeBigNum(result, num_of_ints, output_file);
        }

        free(number1_str);
        free(number2_str);
        free(inter);
        free(num1);
        free(num2);
        free(result);
    } else {
        MPI_Bcast(&mode, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (mode == STATIC) {
            calculateStaticNotRoot();
        }
        if (mode == DYNAMIC) {
            calculateDynamicNotRoot();
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    return 0;
}
