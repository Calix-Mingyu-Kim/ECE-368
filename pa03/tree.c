#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

void Print_Tree(Tnode *root)
{
    if (!root)
        return ;
    if (root->cutline)
        printf("%c\n", root->cutline);
    else
        printf("%d(%d,%d)\n", root->label, root->w, root->h);
    Print_Tree(root->left);
    Print_Tree(root->right);
}

//out_file1
Tnode *Tree_Load_From_File(char *in_file)
{
    FILE *fp = fopen(in_file, "r");

    //fail to open file
    if (fp == NULL){
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return 0;
    }
    fseek(fp, -2, SEEK_END);    // very last element is always an alphabet.
    Tnode *root = Build_BST(fp);
    fclose(fp);

    return root;
}

Tnode *Build_BST(FILE *fp)
{
    // Create node
    Tnode *node = (Tnode*)calloc(1, sizeof(*node));
    if (node == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        return 0;
    } 

    int pos = ftell(fp);
    if (pos < 0)
        return NULL;

    int count = 0;
    do 
    {
        char c = fgetc(fp);
        if (c == '\n' || pos < 0)
            break;
        count++;
        pos -= 1;
        fseek(fp, pos, SEEK_SET);
    } while(1);
    
    if (count < 3)  // if H or V (non-leaf node)
    {    
        char cutline;
        if (fscanf(fp, "%c\n", &cutline) != 1)
            fprintf(stderr, "cutline cannot read\n");

        fseek(fp, pos-1, SEEK_SET);
        node->cutline = cutline;
        node->right = Build_BST(fp);
        node->left = Build_BST(fp);
    }
    else 
    {
        if (pos < 0)
            rewind(fp);
        int label, w, h;
        if (fscanf(fp, "%d(%d,%d)\n", &label, &w, &h) != 3)
            fprintf(stderr, "node cannot read\n");
        node->label = label;
        node->w = w;
        node->h = h;
        node->left = NULL;
        node->right = NULL;
        fseek(fp, pos-1, SEEK_SET);
    }
    return node;
}

int Preorder_Save_To_File(char *filename, Tnode *root, int activateXY)
{
    FILE *fp = fopen(filename, "w");
    //fail to open file
    if (fp == NULL){
        fprintf(stderr, "[ERROR] fopen failed. for save\n");
        return -1;
    }
    Preorder_BST(fp, root, activateXY);
    fclose(fp);
    return 0;
}

void Preorder_BST(FILE *fp, Tnode *root, int activateXY)
{
    if (root == NULL)
        return ;
    // traverse root, left, right
    if (activateXY)    //for outfile_3
    {
        if (root->cutline && root->x)
            fprintf(fp, "%c(%d,%d)\n", root->cutline, root->x, root->y);
        else if (root->label && root->x)
            fprintf(fp, "%d(%d,%d)\n", root->label, root->x, root->y);
        else if(root->label)
            fprintf(fp, "%d\n", root->label);
        else
            fprintf(fp, "%c\n", root->cutline);
    }
    else{
        if (root->cutline && !root->x)
            fprintf(fp, "%c\n", root->cutline);
        else
            fprintf(fp, "%d(%d,%d)\n", root->label, root->w, root->h);
    }
    Preorder_BST(fp, root->left, activateXY);
    Preorder_BST(fp, root->right, activateXY);
}

Tnode *reRootLR_test(Tnode *root)
{
    if (root->left->label)
    {
        Postorder_rebuild_BST(root);
        // printf("LR: Found label: %d! root->w, h: %d, %d\n", root->left->label, root->w, root->h);
        return root;
    }

    Tnode *q = root;
    Tnode *p = root->left;

    q->left = p->left;
    p->left = q;
    root = p;

    return reRootRL_test(p);
}

Tnode *reRootRL_test(Tnode *root)
{
    if (root->right->label)
    {
        Postorder_rebuild_BST(root);
        // printf("LR: Found label: %d! root->w, h: %d, %d\n", root->right->label, root->w, root->h);
        return root;
    }
    
    Tnode *q = root;
    Tnode *p = root->right;

    q->right = p->right;
    p->right = q;
    root = p;

    return reRootLR_test(p);
}

Tnode *reRootAll(Tnode *root)
{
    root = reRootL(root);
    root = reRootR(root);
    root->left->x = root->left->y = '\0';
    root->right->x = root->right->y = '\0';

    return root;
}

Tnode *LL(Tnode *root)
{
    Tnode *q = root;
    Tnode *p = root->left;

    q->left = p->right;
    p->right = q;
    return p;
}

Tnode *LR(Tnode *root)
{
    Tnode *q = root;
    Tnode *p = root->left;

    q->left = p->left;
    p->left = q;
    return p;
}

Tnode *reRootL(Tnode *root)
{
    if (root->left->label)
    {
        Postorder_rebuild_BST(root);
        // printf("L: Found label: %d! root->w, h: %d, %d\n", root->left->label, root->w, root->h);
        root->left->x = root->w;
        root->left->y = root->h;
        return root;
    }
    root = LL(root);
    root = reRootL(root);
    root = RR(root);
    root = LR(root);
    root = reRootR(root);
    root = LR(root);
    Postorder_rebuild_BST(root);
    root->left->x = root->w;
    root->left->y = root->h;
    return root;
}

Tnode *RL(Tnode *root)
{
    Tnode *q = root;
    Tnode *p = root->right;

    q->right = p->right;
    p->right = q;
    return p;
}

Tnode *RR(Tnode *root)
{
    Tnode *q = root;
    Tnode *p = root->right;

    q->right = p->left;
    p->left = q;
    return p;
}


Tnode *reRootR(Tnode *root)
{
    if (root->right->label)
    {
        Postorder_rebuild_BST(root);
        // printf("R: Found label: %d! root->w, h: %d, %d\n", root->right->label, root->w, root->h);
        root->right->x = root->w;
        root->right->y = root->h;
        return root;
    }
    root = RL(root);
    root = reRootL(root);
    root = RL(root);
    root = RR(root);
    root = reRootR(root);
    root = LL(root);
    Postorder_rebuild_BST(root);
    root->right->x = root->w;
    root->right->y = root->h;
    return root;
}

void Postorder_rebuild_BST(Tnode *root)
{
	if (root == NULL) 
        return ;
    if (root->left && !root->left->x)
        Postorder_rebuild_BST(root->left);
    if (root->right && !root->right->x)
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

void Free_Tree(Tnode* node)
{
    if (node == NULL)
        return ;
    Free_Tree(node->left);
    Free_Tree(node->right);
    free(node);
}
