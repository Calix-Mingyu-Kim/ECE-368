#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "graph.h"

int abs(int w) 
{
    return w * ((w>0) - (w<0));
}

int min(int a, int b)
{
    return a < b ? a : b;
}

Edge_Node *add_e_node (Edge_Node *head, Node *node, int w)
{
    Edge_Node *e_temp = head;
    Edge_Node *new_e_node = (Edge_Node *)malloc(sizeof(Edge_Node));
    if (new_e_node == NULL){
        fprintf(stderr, "add_e_node malloc failed\n");
        return 0;
    } 
    new_e_node->e_next = e_temp;
    new_e_node->node = node;
    new_e_node->weight = w;
    return new_e_node;
}

Edge_Node *add_e_node_ (Edge_Node *head, Node *node, int w)
{
    Edge_Node *e_temp = head;
    Edge_Node *new_e_node = (Edge_Node *)malloc(sizeof(Edge_Node));
    // node->column++;
    if (new_e_node == NULL){
        fprintf(stderr, "add_e_node malloc failed\n");
        return 0;
    } 
    new_e_node->e_next = e_temp;
    new_e_node->node = node;
    new_e_node->weight = w;
    return new_e_node;
}
Vertex_Node delete_e_node (Vertex_Node head, int check)
{
    // Edge_Node *e_temp = head.e_next;
    // head.e_next = e_temp->e_next;
    // free(e_temp);
    Edge_Node **prev = &(head.e_next);
    Edge_Node *curr = head.e_next;

    while (curr != NULL) {
        if ((check == 0 && curr->node->column < head.node->column) ||
            (check == 1 && curr->node->column > head.node->column)) {
                *prev = curr->e_next;
                break;
            }
        prev = &(curr->e_next);
        curr = curr->e_next;
    }
    
    free(curr);
    return head;
}

int compute_w (Node *v_node, Node *e_node)
{
    int r = abs(v_node->row - e_node->row); // difference between two rows
    int c = abs(v_node->column - e_node->column);

    if (c == 0)
        return (2 * (r - 1));
    if (r <= 1)
        return (2 * c - 1);
    r = r - 1;
    if (c <= r)
        return (2 * r);
    if (c > r)
        return (2 * c - 1);

    return -1;
}

// int compute_w_start (int row, Node *e_node)
// {
//     int e_row = e_node->row;
//     int c = e_node->column;

//     if (row > e_row) {
//         e_row++;
//     }



// }

Vertex_Node *Create_Graph(int column, int row, FILE *fp, int *v_count) 
{
    // Keep track of row and column
    // Create node when encountered 1
    // Create Adj_list if there is none, else connect to Adj_list
    // Create End_Node when encountered '/n'
    Vertex_Node *v_node = (Vertex_Node*)malloc((row+1) * sizeof(Vertex_Node));
    if (v_node == NULL){
        fprintf(stderr, "v_node malloc failed\n");
        return 0;
    } 

    Node *dummy_node = (Node *)malloc(sizeof(Node));
    if (dummy_node == NULL){
        fprintf(stderr, "dummy_node malloc failed\n");
        return 0;
    } 
    dummy_node->row = -1; dummy_node->column = -1;
    v_node[0].node = dummy_node;
    v_node[0].e_next = NULL;

    // add start nodes to each row
    for (int k = 1; k<row+1; k++) {
        Node *start_node = (Node *)malloc(sizeof(Node));
        if (start_node == NULL){
            fprintf(stderr, "v_node malloc failed\n");
            return 0;
        } 

        start_node->row = k-1; start_node->column = -1;
        v_node[k].node = start_node;
        v_node[k].e_next = NULL;
        *v_count += 1;
    }

    Node *node = NULL;
    Node *prev_node = NULL;
    // bool mid_n = false; 
    int is_node;
    
    for (int r=0; r<row; r++) {
        prev_node = NULL; //mid_n = false; //left_most = NULL; 
        for (int c=0; c<=column; c++) {
            is_node = fgetc(fp); 
            // printf("(%d,%d): %c \n", r, c, is_node);
            if (is_node == '1') {
                node = (Node *)malloc(sizeof(Node));
                if (node == NULL){
                    fprintf(stderr, "node malloc failed\n");
                    return 0;
                } 
                node->row = r; node->column = c;
                node->d = 0;
            
                // Create new_v_node for the new found node
                v_node = (Vertex_Node *)realloc(v_node, (*v_count + 1) * sizeof(Vertex_Node));
                if (v_node == NULL){
                    fprintf(stderr, "v_node realloc failed\n");
                    return 0;
                } 
                v_node[*v_count].node = node;
                v_node[*v_count].e_next = NULL;

                // connect to all start nodes
                if (!prev_node) { 
                    for (int i = 1; i<row+1; i++) {
                        if (node->row == v_node[i].node->row) {
                            v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[i].node, 2*c);
                            v_node[i].e_next = add_e_node(v_node[i].e_next, node, 2*c);    
                        }
                        else if(v_node[i].node->row > node->row && node->column == 0) {
                            v_node[i].node->column = 0;
                            int compute = compute_w(v_node[i].node, node);
                            v_node[i].node->column = -1;
                            if (compute != 0) compute--;
                            else if (compute == 0 && v_node[i].node->row - node->row == -1){
                                compute = 1;
                            }
                            v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[i].node,compute);
                            v_node[i].e_next = add_e_node(v_node[i].e_next, node, compute);
                        }
                        else {
                            v_node[i].node->column = 0;
                            int compute = compute_w(v_node[i].node, node);
                            v_node[i].node->column = -1;
                            if (compute != 0) compute++;
                            else if (compute == 0 && v_node[i].node->row - node->row == -1){
                                compute = 1;
                            }
                            v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[i].node,compute);
                            v_node[i].e_next = add_e_node(v_node[i].e_next, node, compute);
                        }
                        // else {
                        //     v_node[i].node->column = 0;
                        //     v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[i].node, compute_w(v_node[i].node, node));
                        //     v_node[i].e_next = add_e_node(v_node[i].e_next, node, compute_w(v_node[i].node, node));
                        //     v_node[i].node->column = -1;
                        // }
                    }
                    // *v_count += 1;
                    prev_node = node;
                    // break;
                }

                // add immediate precendant node in same row
                if (v_node[*v_count-1].node->row == node->row && v_node[*v_count-1].node->column != -1) {
                    v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[*v_count-1].node, compute_w(v_node[*v_count-1].node, node));
                    v_node[*v_count-1].e_next = add_e_node(v_node[*v_count-1].e_next, node, compute_w(v_node[*v_count-1].node, node));
                }

                short dist_l = __SHRT_MAX__, dist_r = __SHRT_MAX__, dist_m = __SHRT_MAX__;
                int v_prev_l = *v_count, v_prev_r = *v_count;
                // printf("row, column: %d,%d\n", r, c);
                for (int i = *v_count; i >= row; i--) { // put right, mid, left nodes of column immediately above new_v_node
                    if (v_node[i].node->row < node->row && v_node[i].node->column != -1 && v_node[i].node->row != row && v_node[i].node->column != column){
                        short dist_temp = compute_w(v_node[i].node, node);  // # of rotations from new_v_node to v_temp->node 
                        if (c == v_node[i].node->column && v_node[i].node->row == r - 1) { //immediate node above, edge weight is 0
                            v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[i].node, dist_temp);
                            v_node[i].e_next = add_e_node(v_node[i].e_next, node, dist_temp);
                        }
                        else if (v_node[i].node->column == node->column && dist_temp < dist_m){
                            v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[i].node, dist_temp);
                            v_node[i].e_next = add_e_node(v_node[i].e_next, node, dist_temp);
                            dist_m = dist_temp;
                        }
                        else if (v_node[i].node->column < node->column && dist_temp < dist_l) { // if v_temp->node->column is left of new_v_node
                            if (dist_l != __SHRT_MAX__) { // if v_temp->node->row is at the same row of node that is already added, delete the previous added node
                                // printf("node (%d,%d), duplicate, must delete. node: (r,c) = (%d,%d)\n", node->row, node->column, v_temp->node->row, v_temp->node->column);
                                // no need to delete since first found diagonally left node is already the shortest
                                v_node[v_prev_l] = delete_e_node(v_node[v_prev_l], -1);
                                v_node[*v_count] = delete_e_node(v_node[*v_count], 0);
                                v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[i].node, dist_temp);
                                v_node[i].e_next = add_e_node(v_node[i].e_next, node, dist_temp);
                            }
                            else { // else, add edge
                                v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[i].node, dist_temp);
                                v_node[i].e_next = add_e_node(v_node[i].e_next, node, dist_temp);
                                v_prev_l = i;
                            }
                            // update dist_l
                            dist_l = dist_temp;
                        }
                        else if (v_node[i].node->column > node->column && dist_temp < dist_r) { // if v_temp->node->column is right of new_v_node
                            // printf("check!\n");
                            if (dist_r != __SHRT_MAX__) { // if v_temp->node->row is at the same row of node that is already added, delete the previous added node
                                // printf("node: %d,%d, wrong place: v_prev: %d,%d, v[*count]: %d,%d\n", node->row, node->column, v_node[i].node->row, v_node[i].node->column, v_node[*v_count].e_next->node->row, v_node[*v_count].e_next->node->column);
                                v_node[v_prev_r] = delete_e_node(v_node[v_prev_r], -1);
                                // printf("v_count: %d, i: %d, vnode enode delete: %d,%d\n", *v_count, i, v_node[*v_count].e_next->node->row, v_node[*v_count].e_next->node->column);
                                v_node[*v_count] = delete_e_node(v_node[*v_count], 1);
                                // printf("node: %d,%d, v_prev->e_next: %d,%d\n", node->row, node->column, v_node[v_prev].e_next->node->row, v_node[v_prev].e_next->node->column);
                                v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[i].node, dist_temp);
                                v_node[i].e_next = add_e_node(v_node[i].e_next, node, dist_temp);
                                v_prev_r = i;
                            }
                            else { // else, add edge
                                v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[i].node, dist_temp);
                                v_node[i].e_next = add_e_node(v_node[i].e_next, node, dist_temp);
                                // printf("v_count: %d, i: %d, added: vnode->enode: %d,%d\n", *v_count, i, v_node[*v_count].e_next->node->row, v_node[*v_count].e_next->node->column);
                                // printf("v_node[i]: %d,%d, v_node[i].e_next: %d,%d\n", v_node[i].node->row, v_node[i].node->column, v_node[i].e_next->node->row, v_node[i].e_next->node->column);
                                v_prev_r = i;
                                // up_row_r = v_node[i].node->row;
                            }
                            //update dist_r
                            dist_r = dist_temp;
                        }
                    }
                }
                prev_node = node;
                *v_count += 1;
            }

            if (is_node == '\n') {  // create destination nodes, connect by linked list
                // printf("r: %d, c:%d\n", r, c);
                // add node to the rightmost node of the row (if node exists)
                node = (Node *)malloc(sizeof(Node));
                if (node == NULL){
                    fprintf(stderr, "v_node realloc failed\n");
                    return 0;
                } 
                node->row = r; node->column = c;
                node->d = 0;
                v_node = (Vertex_Node *)realloc(v_node, (*v_count + 1) * sizeof(Vertex_Node));
                if (v_node == NULL){
                    fprintf(stderr, "v_node realloc failed\n");
                    return 0;
                } 
                // Vertex_Node new_dest_v_node = v_node[*v_count];
                v_node[*v_count].node = node;
                v_node[*v_count].e_next = NULL;
                v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, v_node[0].node, 0);
                v_node[0].e_next = add_e_node(v_node[0].e_next, node, 0);

                if (prev_node) {    
                    v_node[*v_count-1].e_next = add_e_node(v_node[*v_count-1].e_next, node, compute_w(v_node[*v_count-1].node, node));
                    v_node[*v_count].e_next = add_e_node(v_node[*v_count].e_next, prev_node, compute_w(v_node[*v_count].node, prev_node));
                }              
                *v_count = *v_count + 1;
                break;    
            }   
        }
    }
    // printf("count: %d\n", *v_count);

    // Test V_node and E_edges
    // Vertex_Node *v_temp = v_node;
    // for (int j=0; j<*v_count; j++) {
    //     printf("vertex node: (%d,%d)", v_temp[j].node->row, v_temp[j].node->column);
    //     Edge_Node *e_temp = v_temp[j].e_next;
    //     while (e_temp != NULL) {
    //         printf("| edges: (%d,%d, %d, %d) | ", e_temp->node->row, e_temp->node->column, e_temp->node->d, e_temp->weight);
    //         e_temp = e_temp->e_next;
    //     }
    //     printf("\n");      
    // }

    return v_node;
}

void Free_adj_list (Vertex_Node *v_node, int temp)
{
    for (int j=0; j<temp; j++) {
        Vertex_Node v_temp = v_node[j];
        Edge_Node *e_temp = v_temp.e_next;
        while (e_temp) {
            Edge_Node *temp_e = e_temp;
            e_temp = temp_e->e_next;
            free(temp_e);
        }
        free(v_temp.node);
    }
    free(v_node);
}
