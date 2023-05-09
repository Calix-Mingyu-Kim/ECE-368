#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell_array.h"
#include "shell_list.h"

int main(int argc, char *argv[]) 
{
    int size;
    long n_comp = 0;
    int num_stored = 0;

    if(strcmp(argv[1],"-a") == 0)
    {
        long *arr = Array_Load_From_File(argv[2], &size);
        Array_Shellsort(arr, size, &n_comp);
        if (size == -1) {
            fprintf(stderr, "Cannot open input file\n");
            return EXIT_FAILURE;
        }
        num_stored = Array_Save_To_File(argv[3], arr, size);
        free(arr);
        if (num_stored <= -1) {
            fprintf(stderr, "Cannot open output file\n");
            return EXIT_FAILURE;
        }
        printf("%ld\n", n_comp);
        return EXIT_SUCCESS;

    }
    else if (strcmp(argv[1], "-l") == 0)
    {
        Node *list = List_Load_From_File(argv[2]);
        if (!list) {
            fprintf(stderr, "Cannot open input file\n");
            return EXIT_FAILURE;
        }
        list = List_Shellsort(list, &n_comp);
        num_stored = List_Save_To_File(argv[3], list);

        // free linked list
        while (list != NULL)
        {
            Node *tmp = list;
            list = list->next;
            free(tmp);
        }
        if (num_stored <= -1) {
            fprintf(stderr, "Cannot open output file\n");
            return EXIT_FAILURE;
        }
        printf("%ld\n", n_comp);
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
