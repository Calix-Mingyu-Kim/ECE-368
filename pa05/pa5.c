#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"
#include "dijkstra.h"
#include "output.h"

int main(int argc, char *argv[])
{
    if (argc != 4){
        fprintf(stderr, "argc not 4");
        return EXIT_FAILURE;
    }
    // ----------------------------------------------------------------------- out_file1
    FILE *in_1 = fopen(argv[1], "r");
    //fail to open file
    if (in_1 == NULL){
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return EXIT_FAILURE;
    }
    int row, column;
    if (fscanf(in_1, "%d %d\n", &row, &column) != 2) 
    { 
        fprintf(stderr, "[ERROR] First line of input file format is wrong.\n");
        return EXIT_FAILURE; 
    }
    // printf("row: %d, column: %d\n", row, column);

    int *v_count;
    int c_temp = 1;
    v_count = &c_temp;

    Vertex_Node *v_node = Create_Graph(column, row, in_1, v_count);
    Min_Heap *min_heap = (Min_Heap *)malloc(*v_count * sizeof(*min_heap));
    if (min_heap == NULL){
        fprintf(stderr, "min_heap malloc failed\n");
        return 0;
    } 

    // Create_PQ (min_heap, v_node, idx, 0, *v_count - 1);
    Create_PQ(min_heap, v_node, v_count);
    int temp = *v_count;
    dijkstra(min_heap, v_count);
    // for (int j=0; j<temp; j++)
    //     printf("%d,%d, %d, %d\n", min_heap[j].v_node.node->row, min_heap[j].v_node.node->column, min_heap[j].v_node.node->heap_idx, min_heap[j].v_node.node->d);    
    
    fclose(in_1);
    FILE *out_1 = fopen(argv[2], "w");
    FILE *out_2 = fopen(argv[3], "w");
    out_file1(out_1, out_2, min_heap, row, column, temp);
    fclose(out_1);
    fclose(out_2);
    // ----------------------------------------------------------------------- out_file2

    free(min_heap);
    Free_adj_list(v_node, temp);
    
    return EXIT_SUCCESS;
}
