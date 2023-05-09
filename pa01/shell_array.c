#include <stdio.h>
#include <stdlib.h>

#include "sequence.h"
#include "shell_array.h"

long *Array_Load_From_File(char *filename, int *size){
    FILE *fp = fopen(filename, "rb");

    //fail to open file
    if (fp == NULL){
        //return NULL address and assign 0 to *size
        fprintf(stderr, "[ERROR] fopen failed.\n");
        *size = 0;
        return NULL;
    }
    fseek(fp, 0, SEEK_END); //go to end of file

    *size = ftell(fp) / sizeof(long);
    // printf("Address of size variable: %p\n", size);
    // printf("Total size of file.txt = %d\n", *size);
    long *arr = (long *)malloc(sizeof(long) * (*size)); 

    //fail to allocate sufficient memory
    if (arr == NULL)
    {
        //return NULL address and assign 0 to *size
        fprintf(stderr, "[ERROR] malloc failed.\n");
        *size = 0;
        return NULL;
    }
    
    fseek(fp, 0, SEEK_SET); //go to start of file
    fread(arr, sizeof(long), *size, fp);
    fclose(fp);
    
    return arr;
}

int Array_Save_To_File(char *filename, long *array, int size)
{
    FILE *fp = fopen(filename, "wb");

    //fail to open file
    if (fp == NULL){
        //return NULL address and assign 0 to *size
        fprintf(stderr, "[ERROR] fopen failed.\n");
        size = 0;
        return 0;
    }
    //If array is NULL or size is 0
    if (!array || size == 0)
    {
        fclose(fp);
        return 0;
    }
    int saved = fwrite(array, sizeof(long), size, fp);
    fclose(fp);
    return saved;
}

void Array_Shellsort(long *array, int size, long *n_comp)
{
    // Generate sequence for shell sort
    int seq_size;
    long *sequence = Generate_2p3q_Seq(size, &seq_size);

    // Shell sort using insertion sort
    for (int x=seq_size-1; x>=0; x--)
    {
        long k = sequence[x];
        for (int j=k; j<size; j++)
        {
            long temp = array[j];
            int i = j;
            *n_comp += 1;
            while (i>=k && array[i-k] > temp)
            {
                array[i] = array[i-k];
                i = i-k;
            }
            array[i] = temp;
        }
    }
    free(sequence);
}
