#ifndef __TREE_H__
#define __TREE_H__

typedef struct _Tnode {
   char cutline;
   int label, w, h, x, y;
   struct _Tnode *left;
   struct _Tnode *right;
} Tnode;

void Print_Tree(Tnode *root);
//out_file1
Tnode *Tree_Load_From_File(char *in_file);
Tnode *Build_BST(FILE *fp);
int Preorder_Save_To_File(char *filename, Tnode *root, int activateXY);
void Preorder_BST(FILE *fp, Tnode *root, int activateXY);
//out_file2
Tnode *reRootLR(Tnode *root);
Tnode *reRootRL(Tnode *root);

void update_xy_right(Tnode *p, Tnode *q); // for LL and RL
void update_xy_right(Tnode *p, Tnode *q);
Tnode *LR(Tnode *root, int update);
Tnode *RL(Tnode *root, int update);
Tnode *LL(Tnode *root, int update);
Tnode *RR(Tnode *root, int update);

Tnode *reRootAll(Tnode *root);
Tnode *reRootL(Tnode *root);
Tnode *reRootR(Tnode *root);

void Postorder_rebuild_BST(Tnode *root);

void Free_Tree(Tnode* node);

#endif