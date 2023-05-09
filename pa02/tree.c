#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

//out_file1
Tnode *Tree_Load_From_File(char *in_file)
{
    FILE *fp = fopen(in_file, "r");

    //fail to open file
    if (fp == NULL){
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return 0;
    }
    Tnode *root = Build_BST(fp);
    fclose(fp);

    return root;
}

Tnode *Build_BST(FILE *fp)
{
    // Create node
    Tnode *node = (Tnode*)calloc(1, sizeof(*node));

    if (node == NULL){
        fprintf(stderr, "malloc failed\n");
        return 0;
    } 
    int label, w, h;
    int check = fscanf(fp, "%d(%d,%d)\n", &label, &w, &h);
    if (check == -1)
        return NULL;
    if (check == 3)
    {
        node->label = label;
        node->w = w;
        node->h = h;
    }
    else
    {
        char cutline;
        check = fscanf(fp, "%c\n", &cutline);
        node->cutline = cutline;
    }
    
    if (check != 3)    // if H or V (non-leaf node)
    {     
        node->left = Build_BST(fp);
        node->right = Build_BST(fp);
    }    
    else
    {
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

int Postorder_Save_To_File(char *filename, Tnode *root)
{
    FILE *fp = fopen(filename, "w");
    //fail to open file
    if (fp == NULL){
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return -1;
    }
    Postorder_BST(fp, root);
    fclose(fp);
    return 0;
}

void Postorder_BST(FILE *fp, Tnode *root)
{
    if (root == NULL)
        return ;
    // traverse left, right, root
    Postorder_BST(fp, root->left);
    Postorder_BST(fp, root->right);
    if (root->cutline)
        fprintf(fp, "%c\n", root->cutline);
    else
        fprintf(fp, "%d(%d,%d)\n", root->label, root->w, root->h);
}

//out_file2
void Postorder_rebuild_BST(Tnode *root)
{
	if(root == NULL) 
        return ;

	Postorder_rebuild_BST(root->left);
    Postorder_rebuild_BST(root->right);   
    // if V or H, visit root->left and root->right, add accordingly.
    if (root->cutline)
    {
        int width_l = root->left->w;
        int height_l = root->left->h;

        int width_r = root->right->w;
        int height_r = root->right->h;
        
        if (root->cutline == 'H')   // Horizontal cut. (max of width, sum of height)
        {
            root->w = (width_l > width_r) ? width_l : width_r;
            root->h = height_l + height_r;
        }
        else    // Vertical cut
        {
            root->w = width_l + width_r;
            root->h = (height_l > height_r) ? height_l : height_r;
        }
    }
}

int Postorder_Save_To_File_2(char *filename, Tnode *root)
{
    FILE *fp = fopen(filename, "w");
    //fail to open file
    if (fp == NULL){
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return 0;
    }
    Postorder_BST_2(fp, root);
    fclose(fp);
    return 1;
}

void Postorder_BST_2(FILE *fp, Tnode *root)
{
    if (root == NULL)
        return ;
    // traverse left, right, root
    Postorder_BST_2(fp, root->left);
    Postorder_BST_2(fp, root->right);
    if (root->cutline == 'V' || root->cutline == 'H')
        fprintf(fp, "%c(%d,%d)\n", root->cutline, root->w, root->h);
    else
        fprintf(fp, "%d(%d,%d)\n", root->label, root->w, root->h);
}

//out_file3
void Assign_Coord(Tnode *root)
{
    int saved_x = 0;
    int saved_y = 0;
    Find_Origin_Node(root, saved_x, saved_y, 0);
}

void Find_Origin_Node(Tnode *node, int saved_x, int saved_y, int unvisited_node)
{
    if (node == NULL)
        return ;
    // if node == 'V', go to left of its node
    if (node->cutline == 'V')
    {
        Find_Origin_Node(node->left, saved_x, saved_y, unvisited_node);
    }
    // if node == 'H', go to right of its node
    else if (node->cutline == 'H')
    {
        Find_Origin_Node(node->right, saved_x, saved_y, unvisited_node);
    }

    // if node 'V', post order starting left.
    if (node->cutline == 'V')
    {
        if (!unvisited_node)    //if unvisited
        {
            saved_y = 0;   
            saved_x = node->left->w;
        }   
        else 
        {
            saved_x += node->left->w;
        }

        node->x = node->left->w + saved_x;
        node->y = node->left->h + saved_y;
    }
    // if node 'H', post order starting right.
    else if (node->cutline == 'H')
    {
        if (!unvisited_node)
        {
            saved_x = 0;   
            saved_y = node->right->h;
        }
        else 
            saved_y += node->right->h;

        node->x = node->right->w + saved_x;
        node->y = node->right->h + saved_y;
    }
    else
    {
        node->x = saved_x;
        node->y = saved_y;
    }

    // visit unvisited nodes
    if (node->cutline == 'V') 
    {
        Find_Origin_Node(node->right, saved_x, saved_y, 1);
    }
    else if (node->cutline == 'H') // go to right node to find a label to assign coordinate
    {
        Find_Origin_Node(node->left, saved_x, saved_y, 1);
    }
    
}

int Postorder_Save_To_File_3(char *filename, Tnode *root)
{
    FILE *fp = fopen(filename, "w");
    //fail to open file
    if (fp == NULL){
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return 0;
    }
    Postorder_BST_only_label(fp, root);
    fclose(fp);

    return 1;
}

void Postorder_BST_only_label(FILE *fp, Tnode *root)
{
    if (root == NULL)
        return ;
    // traverse left, right, root
    Postorder_BST_only_label(fp, root->left);
    Postorder_BST_only_label(fp, root->right);
    if (!root->cutline)
        fprintf(fp, "%d((%d,%d)(%d,%d))\n", root->label, root->w, root->h, root->x, root->y);
    return ;
}

void Free_Tree(Tnode* node)
{
    if (node == NULL)
        return ;
    Free_Tree(node->left);
    Free_Tree(node->right);
    free(node);
}
