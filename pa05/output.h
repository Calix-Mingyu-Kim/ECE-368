#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "graph.h"
#include "dijkstra.h"

void out_file1(FILE *fp1, FILE *fp2, Min_Heap *min_heap, int row, int column, int n_count);
void print_out_file1(FILE *fp, Node *node, int row, int column);

#endif
