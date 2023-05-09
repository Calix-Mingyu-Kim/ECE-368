#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

void Print_Tree(Tnode *root)
{
    if (!root)
        return ;
    if (root->cutline)
        printf("%c(%d,%d)\n", root->cutline, root->w, root->h);
    else
        printf("%d(%d,%d)\n", root->label, root->w, root->h);

    Print_Tree(root->left);
    Print_Tree(root->right);
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
    else {
        if (root->cutline && !root->x)
            fprintf(fp, "%c\n", root->cutline);
        else
            fprintf(fp, "%d(%d,%d)\n", root->label, root->w, root->h);
    }
    Preorder_BST(fp, root->left, activateXY);
    Preorder_BST(fp, root->right, activateXY);
}

void update_wh(Tnode *node)
{
    int width_l = node->left->w;
    int height_l = node->left->h;

    int width_r = node->right->w;
    int height_r = node->right->h;
    
    if (node->cutline == 'H') {  // Horizontal cut. (max of width, sum of height)
        node->w = (width_l > width_r) ? width_l : width_r;
        node->h = height_l + height_r;
    }
    else {   // Vertical cut
        node->w = width_l + width_r;
        node->h = (height_l > height_r) ? height_l : height_r;
    }
}

void update_xy_left(Tnode *p, Tnode *q)
{
    p->left->x = p->w;
    p->left->y = p->h;
}

void update_xy_right(Tnode *p, Tnode *q)
{
    p->right->x = p->w;
    p->right->y = p->h;
}

Tnode *reRootLR(Tnode *root)
{
    if (root->left->label) {
        return root;
    }

    Tnode *q = root;
    Tnode *p = root->left;

    q->left = p->left;
    p->left = q;
    // update_wh(q);
    // update_wh(p);
    // update_xy_right(p,q);

    return reRootRL(p);
}

Tnode *reRootRL(Tnode *root)
{
    if (root->right->label) {
        return root;
    }
    
    Tnode *q = root;
    Tnode *p = root->right;

    q->right = p->right;
    p->right = q;
    // update_wh(q);
    // update_wh(p);
    // update_xy_left(p,q);

    return reRootLR(p);
}

Tnode *reRootAll(Tnode *root)
{
    Postorder_rebuild_BST(root);

    if (root->left->cutline) {
        root = LL(root,1);
        root = RR(root,0);
    }
    if (root->left->cutline) {
        root = LR(root,1);
        root = LR(root,0);
    }
    if (root->right->cutline) {
        root = RL(root,1);
        root = RL(root,0);
    }
    if (root->right->cutline) {
        root = RR(root,1);
        root = LL(root,0);
    }

    return root;
}

Tnode *LL(Tnode *root, int update)
{
    if (root->left->label)
        return root;

    Tnode *q = root;
    Tnode *p = root->left;

    q->left = p->right;
    p->right = q;

    update_wh(q);
    update_wh(p);
    if (update) {
        update_xy_left(p,q);
        Tnode *temp = p;
        p = LL(p,1);
        if (temp != p)
            p = RR(p,0);
        
        temp = p;
        p = LR(p,1);
        if (temp != p)
            p = LR(p,0);
    }

    return p;
}

Tnode *LR(Tnode *root, int update)
{
    if (root->left->label)
        return root;

    Tnode *q = root;
    Tnode *p = root->left;

    q->left = p->left;
    p->left = q;

    update_wh(q);
    update_wh(p);
    if (update) {
        update_xy_right(p,q);
        Tnode *temp = p;
        p = RL(p,1);
        if (temp != p)
            p = RL(p,0);
        temp = p;
        p = RR(p,1);
        if (temp != p)
            p = LL(p,0);
    }

    return p;
}

Tnode *RL(Tnode *root, int update)
{
    if (root->right->label)
        return root;
    Tnode *q = root;
    Tnode *p = root->right;

    q->right = p->right;
    p->right = q;

    update_wh(q);
    update_wh(p);
    if (update) {
        update_xy_left(p,q);
        Tnode *temp = p;
        p = LL(p,1);
        if (temp != p)
            p = RR(p,0);
        temp = p;
        p = LR(p,1);
        if (temp != p)
            p = LR(p,0);
    }
    return p;
}

Tnode *RR(Tnode *root, int update)
{
    if (root->right->label)
        return root;
    Tnode *q = root;
    Tnode *p = root->right;

    q->right = p->left;
    p->left = q;

    update_wh(q);
    update_wh(p);
    if (update) {
        update_xy_right(p,q);
        Tnode *temp = p;
        p = RL(p,1);
        if (temp != p)
            p = RL(p,0);
        temp = p;
        p = RR(p,1);
        if (temp != p)
            p = LL(p,0);
    }
    return p;
}

void Postorder_rebuild_BST(Tnode *root)
{
    // printf("BST currently in: %d/%c\n", root->label, root->cutline);
	if (root == NULL) 
        return ;
    if (root->left)
        Postorder_rebuild_BST(root->left);
    if (root->right)
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
