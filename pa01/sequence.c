/*
Here, n is the number of long integers to be sorted. 
You should determine the number of elements in the sequence and store that number in *seq size. 
For example, if n is 0 or 1, the sequence should contain 0 elements. 
Therefore, *seq size should store 0. 
For n = 16, the sequence should contain 8 elements. Therefore, *seq size should store 8.
The function should allocate space to store the elements of the sequence as long integers. 
Even when the sequence is empty, you should allocate a space of 0 elements.
Moreover, these elements must be stored in ascending order. The address of the long array is returned. 
If malloc fails, you should return NULL and store 0 in *seq size.
This function will be called by the Array Shellsort and List Shellsort functions. 
It is important that the caller function, 
e.g., Array Shellsort or List Shellsort function, has an integer variable to store the size of the sequence,
 and pass the address of this variable into long *Generate 2p3q Seq(int n, int *seq size) using the seq size parameter.
Any helper functions for Generate 2p3q Seq, if any, must reside in sequence.c. 
These helper func- tions should be declared as static. */

#include <stdio.h>
#include <stdlib.h>

long *Generate_2p3q_Seq(int n, int *seq_size)
{
    // 1, 2, 3, 4, 6, 8, 9, 12, 16, 18, 24, 27 ...
    /*
          1
         2,3
        4,6,9
      8,12,18,27 
     16,24,36,54,81
   32,48,72,108,162,243
   64
    */
    long *sequence = NULL;
    *seq_size = 0;
    if (n < 2) sequence = (long *)malloc(0);
    else
    {
        int q = 0, row = 0, z = 0, n1 = 1, n2 = 1;
        int count = 0;
        while(n1 * n2 < n) {
            sequence = realloc(sequence, (count + 1) * sizeof(long));
            sequence[count] = n1 * n2;
            count++;
            q++;
            n1 /= 2;
            n2 *= 3;

            if (q > row || n1 * n2 >= n) { 
                row++; 
                q = 0;
                n2 = 1;
                for (n1 = 1, z = 0; z < row; z++) 
                    n1 *= 2;
            }
        }
        *seq_size = count;
        // Sort the sequence in ascending order
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (sequence[j] > sequence[j + 1]) {
                    int temp = sequence[j];
                    sequence[j] = sequence[j + 1];
                    sequence[j + 1] = temp;
                }
            }
        }
        // // Print the sequence
        // for (int i = 0; i < count; i++) {
        //     printf("%ld ", sequence[i]);
        // }
        // printf("\n");
    }
    return sequence;
}
