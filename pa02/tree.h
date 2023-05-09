#ifndef __TREE_H__
#define __TREE_H__

typedef struct _Tnode {
   char cutline;
   int label, w, h, x, y;
   struct _Tnode *left;
   struct _Tnode *right;
} Tnode;

//out_file1
Tnode *Tree_Load_From_File(char *in_file);
Tnode *Build_BST(FILE *fp);
int Postorder_Save_To_File(char *filename, Tnode *root);
void Postorder_BST(FILE *fp, Tnode *root);

//out_file2
void Postorder_rebuild_BST(Tnode *root);
int Postorder_Save_To_File_2(char *filename, Tnode *root);
void Postorder_BST_2(FILE *fp, Tnode *root);

//out_file3
void Assign_Coord(Tnode *root);
void Find_Origin_Node(Tnode *node, int saved_x, int saved_y, int unvisited_node);
int Postorder_Save_To_File_3(char *filename, Tnode *root);
void Postorder_BST_only_label(FILE *fp, Tnode *root);

void Free_Tree(Tnode* node);

#endif
