#ifndef __EVAL_H__
#define __EVAL_H__

void Print_Tree(Tnode *node);

int Binary_to_txt_key(FILE *fp, int *open_success);
char Binary_to_txt_balance(FILE *fp, int *open_success);

void Text_to_Binary_File(char *input_file, char *output_file);
void Binary_to_Text_File(char *input_file, char *output_file);
Tnode *Build_BST_e(FILE *fp, int *open_success);

int Check_BST(Tnode *root, int min, int max);

int Check_Height_balanced(Tnode *root);

void Free_Tree(Tnode* node);

#endif