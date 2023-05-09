#ifndef __BUILD_H__
#define __BUILD_H__

Tnode *Build_BST_b(FILE *fp, int *open_success);
Tnode *Search_and_insert(Tnode **root, int key);

Tnode *Find_Predecessor(Tnode *curr, Tnode *prev, Tnode *p_predecessor, Tnode *predecessor, int *update_h);
Tnode *Check_Balance(Tnode *parent); 

Tnode *Delete_and_balance(Tnode *curr, Tnode *prev, int key, int *update_h);
Tnode *Search_and_delete(Tnode **root, int key);

Tnode *CR(Tnode *old_root);
Tnode *CCR(Tnode *old_root);

// int Preorder_Save_To_File(char *filename, Tnode *root);
void Preorder_BST(FILE *fp, Tnode *root);

#endif
