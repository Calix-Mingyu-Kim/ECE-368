#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "graph.h"
#include "dijkstra.h"

void Create_PQ(Min_Heap *min_heap, Vertex_Node *v_node, int *idx)
{
    for (int i=0; i<*idx; i++) {
        min_heap[i].v_node = v_node[i];
        min_heap[i].v_node.node->heap_idx = i;
        min_heap[i].v_node.node->d = __SHRT_MAX__;
        min_heap[i].v_node.node->pred = NULL;
    }

    // for (int j=0; j<*idx; j++)
    //     printf("%d,%d, %d\n", min_heap[j].v_node.node->row, min_heap[j].v_node.node->column, min_heap[j].v_node.node->heap_idx);
}

void Upward_heapify (Min_Heap *PQ, int n)
{
    int new = PQ[n].v_node.node->d;
    int child = n;
    int parent = (child - 1) / 2;
    while (child > 0 && PQ[parent].v_node.node->d >= new) {
        // printf("parent, head_idx: (%d,%d, %d)\n", PQ[parent].v_node.node->row, PQ[parent].v_node.node->column, PQ[parent].v_node.node->heap_idx);
        // printf("child, head_idx: (%d,%d, %d)\n", PQ[child].v_node.node->row, PQ[child].v_node.node->column, PQ[child].v_node.node->heap_idx);
        Min_Heap temp = PQ[child];
        PQ[child] = PQ[parent];
        PQ[child].v_node.node->heap_idx = child; 
        PQ[parent] = temp;
        PQ[parent].v_node.node->heap_idx = parent;
        child = parent;
        parent = (child - 1) / 2;
    }
    // PQ[child] = _temp;
    PQ[child].v_node.node->d = new;
    PQ[child].v_node.node->heap_idx = child;
}

void Downward_heapify (Min_Heap *PQ, int i, int n)
{
    Min_Heap temp = PQ[i];
    int j;
    while ((j = 2*i+1) <= n) { //if there is a left child
        if (j < n && PQ[j].v_node.node->d > PQ[j+1].v_node.node->d) 
            j = j+1;
        if (temp.v_node.node->d <= PQ[j].v_node.node->d)
            break;
        else {
            // printf("j!! = %d, n!! = %d\n", j, n);
            Min_Heap _temp = PQ[i];
            PQ[i] = PQ[j];
            PQ[i].v_node.node->heap_idx = i;
            PQ[j] = _temp;
            PQ[j].v_node.node->heap_idx = j;
            i = j;
        }
    }
    PQ[i] = temp;
    PQ[i].v_node.node->heap_idx = (j-1)/2;
}

int Extract_Min(Min_Heap *PQ, int *idx)
{
    // swap, dequeue 
    // find minimum
    int min = *idx - 1;
    *idx -= 1;
    // printf("swapped, current PQ[0]: (%d,%d), current PQ[*idx-1]: (%d,%d), *idx: %d\n", PQ[0].v_node.node->row, PQ[0].v_node.node->column, PQ[*idx].v_node.node->row, PQ[*idx].v_node.node->column, *idx);
    Min_Heap heap_temp = PQ[0];
    PQ[0] = PQ[*idx];
    PQ[0].v_node.node->heap_idx = 0;
    PQ[*idx] = heap_temp;
    PQ[*idx].v_node.node->heap_idx = *idx;
    // printf("after swap, current PQ[0]: (%d,%d), current PQ[*idx-1]: (%d,%d), *idx: %d\n", PQ[0].v_node.node->row, PQ[0].v_node.node->column, PQ[*idx].v_node.node->row, PQ[*idx].v_node.node->column, *idx);
    
    // printf("===============before downward_heapify\n");
    // for (int j=0; j<*idx; j++)
    //     printf("%d,%d, %d, %d\n", PQ[j].v_node.node->row, PQ[j].v_node.node->column, PQ[j].v_node.node->heap_idx, PQ[j].v_node.node->d);

    Downward_heapify(PQ, 0, min-1);

    // printf("================after downward_heapify\n");
    // for (int j=0; j<*idx; j++)
    //     printf("%d,%d, %d, %d\n", PQ[j].v_node.node->row, PQ[j].v_node.node->column, PQ[j].v_node.node->heap_idx, PQ[j].v_node.node->d);
    
    return min;
}


void dijkstra(Min_Heap *PQ, int *idx) //s: starting node
{
    // for (int u = 0; u<b; u++) {
    //     PQ[u].v_node->node->d = __SHRT_MAX__; PQ[u].v_node->node->pred = NULL;
    // }
    PQ[0].v_node.node->d = 0;    //starting node is always located at the start
    // int check = 0;
    while (*idx != 0) {
        short u = Extract_Min(PQ, idx);
        Edge_Node *e_node = PQ[u].v_node.e_next;
        // printf("PQ[0].v_node.node->d: %d, enode: %d,%d, %d, %d\n", PQ[u].v_node.node->d, e_node->node->row, e_node->node->column, e_node->weight, e_node->node->heap_idx);
        for (short v = e_node->node->heap_idx; e_node != NULL; e_node = e_node->e_next) {
            v = e_node->node->heap_idx;
            // printf("v: %d, (%d,%d) %d > %d + %d\n", v, PQ[v].v_node.node->row, PQ[v].v_node.node->column, PQ[v].v_node.node->d, PQ[u].v_node.node->d, e_node->weight);
            if (PQ[v].v_node.node->d > PQ[u].v_node.node->d + e_node->weight) {
                // printf("enode: %d,%d, %d\n", e_node->node->row, e_node->node->column, e_node->weight);
                PQ[v].v_node.node->d = PQ[u].v_node.node->d + e_node->weight;
                e_node->node->pred = PQ[u].v_node.node;
                // printf("===============before upward_heapify\n");
                // for (int j=0; j<*idx; j++)
                //     printf("%d,%d, %d, %d\n", PQ[j].v_node.node->row, PQ[j].v_node.node->column, PQ[j].v_node.node->heap_idx, PQ[j].v_node.node->d);
                Upward_heapify(PQ, v);
                // printf("===============after upward_heapify\n");
                // for (int j=0; j<*idx; j++)
                //     printf("%d,%d, %d, %d\n", PQ[j].v_node.node->row, PQ[j].v_node.node->column, PQ[j].v_node.node->heap_idx, PQ[j].v_node.node->d);
            }
        }
    }
}
