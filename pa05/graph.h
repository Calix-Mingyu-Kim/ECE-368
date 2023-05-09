#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef struct _Node {
  int row, column, d;
  short heap_idx;
  struct _Node *pred;
} Node; // 24 bytes

typedef struct _Vertex_Node {
  struct _Node *node;
  struct _Edge_Node *e_next;
} Vertex_Node;  // 160 bytes

typedef struct _Edge_Node {
  int weight;
  struct _Node *node;
  struct _Edge_Node *e_next;
} Edge_Node;  //56 bytes

int abs(int w);
int min(int a, int b);
Edge_Node *add_e_node (Edge_Node *head, Node *node, int w);
Vertex_Node delete_e_node (Vertex_Node head, int check);
int compute_w (Node *v_node, Node *e_node);
Vertex_Node *Create_Graph(int row, int column, FILE *fp, int *v_count); 
void Free_adj_list (Vertex_Node *v_node, int temp);

#endif
