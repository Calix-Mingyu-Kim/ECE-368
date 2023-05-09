#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include "graph.h"

// typedef struct _Min_Heap {
//   struct _Vertex_Node *v_node;
//   struct _Min_Heap *left;
//   struct _Min_Heap *right;
//   int heap_idx, d;
// } Min_Heap;

typedef struct _Min_Heap {
  struct _Vertex_Node v_node;
} Min_Heap;


// Vertex_Node *Create_PQ (Min_Heap **min_heap, Vertex_Node *v_node, int *idx, int start, int end);
void Create_PQ(Min_Heap *min_heap, Vertex_Node *v_node, int *idx);
void Upward_heapify (Min_Heap *PQ, int n);
void Downward_heapify (Min_Heap *PQ, int i, int n);
int Extract_Min(Min_Heap *PQ, int *idx);
void dijkstra(Min_Heap *PQ, int *idx);

#endif
