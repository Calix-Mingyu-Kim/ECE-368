#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "graph.h"
#include "dijkstra.h"
#include "output.h"

void out_file1(FILE *fp1, FILE *fp2, Min_Heap *min_heap, int row, int column, int n_count) 
{
    Node *min_dest = NULL;
    short min_d = __SHRT_MAX__;
    // Node *path_to_dest = (Node *)calloc(1, sizeof(Node)* row);
    int *path_to_dest = (int *)malloc(row * sizeof(int));
    if (path_to_dest == NULL){
        fprintf(stderr, "path_to_dest malloc failed\n");
        return ;
    } 

    int temp = 0;
    for (int i=0; i<n_count; i++){
        // fprintf(fp2, "%d\n", min_heap[i].v_node.node->d);
        int dist = __SHRT_MAX__;
        if (min_heap[i].v_node.node->row != -1 && min_heap[i].v_node.node->column == -1){
            Edge_Node *e_temp = min_heap[i].v_node.e_next;
            // printf("for v_node %d,%d\n", min_heap[i].v_node.node->row, min_heap[i].v_node.node->column);
            while (e_temp) {
                // printf("%d,%d, %d, %d\n", e_temp->node->row, e_temp->node->column, e_temp->node->d, e_temp->weight);
                if ((e_temp->weight + e_temp->node->d) < dist)
                    dist = e_temp->weight + e_temp->node->d;
                e_temp = e_temp->e_next;
            }
            path_to_dest[min_heap[i].v_node.node->row] = dist;
            // min_dest = min_heap[7].v_node.node;
            // printf("%d, %d,%d, %d, %d\n", i, min_heap[i].v_node.node->row, min_heap[i].v_node.node->column, min_heap[i].v_node.node->d ,min_heap[i].v_node.node->heap_idx);
            if (min_heap[i].v_node.node->d < min_d) {
                min_dest = min_heap[i].v_node.node;
                min_d = min_heap[i].v_node.node->d;
            }
            temp++;
        }
        if (temp == row+1) break;
    }

    for (int j=0; j<temp; j++){
        fprintf(fp2, "%d\n", path_to_dest[j]);
    }
    free(path_to_dest);
    // while (min_dest) {
    //     printf("pred: %d,%d\n", min_dest->row, min_dest->column);
    //     min_dest = min_dest->pred;
    // }
    // printf("dest nodes: %d,%d\n", min_dest->row, min_dest->column);
    print_out_file1(fp1, min_dest, row, column);
}
void print_out_file1(FILE *fp, Node *node, int row, int column) 
{
    while (node) {
        if ((node->column == -1 && node->row == -1) || node->column == column) break;   // (-1,-1) dummy node
        
        int r_temp = node->row, c_temp = node->column;
        int r_dest = node->pred->row, c_dest = node->pred->column;
        // fprintf(fp, "---------------------------------\ntemp: (%d,%d), dest: (%d,%d)\n", r_temp, c_temp, r_dest, c_dest);

        if (c_temp == -1) {
            //horizontal
            // fprintf(fp, "======== inside c_temp == -1 ========\n");
            fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp, c_temp + 1);
            c_temp += 1;
            if (r_temp > r_dest)
                r_temp -= 1;
        }
        if (r_temp > r_dest && abs(r_temp - r_dest) != 1 && !(c_dest == c_temp))
            r_dest += 1;    

        if (r_temp > r_dest && c_temp < c_dest) { // going right up
            // fprintf(fp, "======== going right up ========\n");
            while (1) { // #1, go |-|-  
                if (r_temp == r_dest || c_temp == c_dest) {
                    break;
                }
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp + 1, c_temp); //vertical 
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp, c_temp + 1); //horizontal
                r_temp -= 1;
                c_temp += 1;
            }
            // adjust = true;
        }
        else if (r_temp < r_dest && c_temp < c_dest) { // going right down
            // fprintf(fp, "======== going right down ========\n");
            while (1) { // #2, go |_|_
                if (c_temp == c_dest || r_temp == r_dest) {
                    break;
                }
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp + 1, c_temp); //vertical 
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp + 1, c_temp, r_temp + 1, c_temp + 1); //horizontal
                r_temp += 1;
                c_temp += 1;
            }
        }
        else if (r_temp > r_dest && c_temp > c_dest) {  // going left up
            // fprintf(fp, "======== going left up ========\n");
            while (1) { // #3, go -|-|
                if (c_temp == c_dest || r_temp == r_dest) {
                    break;
                }
                
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp + 1, c_temp); //vertical 
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp - 1, r_temp, c_temp); //horizontal
                r_temp -= 1;
                c_temp -= 1;
            }
            // adjust = true;
        }
        else if (r_temp < r_dest && c_temp > c_dest) { // going left down
            // fprintf(fp, "======== going left down ========\n");
            while (1) { // #4, go _|_|
                if (c_temp == c_dest || r_temp == r_dest) {
                    break;
                }
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp + 1, c_temp); //vertical 
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp - 1, r_temp, c_temp); //horizontal
                r_temp += 1;
                c_temp -= 1;
            }
        }
        while (r_temp == r_dest) {
            if (c_temp == c_dest) break;
            if (c_temp < c_dest) {  // go right straight
                // fprintf(fp, "======== going right straight ========\n");
                //vertical 
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp + 1, c_temp);
                if (abs(r_temp - r_dest) == 1) break;
                //horizontal
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp, c_temp + 1);
                c_temp += 1;
            }
            else {  // go left straight
                // fprintf(fp, "======== going left straight ========\n");
                // vertical 
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp + 1, c_temp);
                if (abs(r_temp - r_dest) == 1) break;
                //horizontal
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp - 1, r_temp, c_temp);
                c_temp -= 1;
            }
        }
        while (c_temp == c_dest) {
            // if (abs(r_temp - r_dest) == 1){
            //     if ((r_temp + 1) == row) break;
            //     fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp + 1, c_temp);
            //     break;
            // }
            if (r_temp == r_dest) break;
            if (r_temp > r_dest) {  //go up
                // fprintf(fp, "======== going up ========\n");
                //vertical 
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp + 1, c_temp);
                if (abs(r_temp - r_dest) == 1) break;
                //horizontal
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp, c_temp + 1);
                r_temp -= 1;
            }
            else {  // go down
                // fprintf(fp, "======== going down ========\n");
                // vertical 
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp, c_temp, r_temp + 1, c_temp);
                if (abs(r_temp - r_dest) == 1) break;
                //horizontal
                fprintf(fp, "(%d,%d)(%d,%d)\n", r_temp + 1, c_temp, r_temp + 1, c_temp + 1);
                r_temp += 1;
            }
        }

        node = node->pred;
    }
}
