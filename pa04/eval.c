#include <stdio.h>
#include <stdlib.h>

#include "hbt.h"

void Print_Tree(Tnode *node)
{
    if (!node)
        return ;
    printf("%d %d\n", node->key, node->balance);

    Print_Tree(node->left);
    Print_Tree(node->right);
}

int Binary_to_txt_key(FILE *fp, int *open_success)
{
    int key;
    if (fread(&key, sizeof(int), 1, fp) == 1) {
        // printf("Read: %d ", key);=0-
        return key;
    }
    // wrong format
    else if (!feof(fp) && (fread(&key, sizeof(int), 1, fp) != 1)) {
        *open_success = 0;
        return 0;
    }

    return key;
}

char Binary_to_txt_balance(FILE *fp, int *open_success)
{
    char balance;
    // check format "%d %c\n"
    if (fread(&balance, sizeof(char), 1, fp) == 1) {
        if (balance == 'i' || balance == 'd')
            return balance;
        else
            return (int) balance;
    }
    // wrong format
    else if (!feof(fp) && fread(&balance, sizeof(char), 1, fp) != 1) {
        *open_success = 0;
        return 0;
    }
    return balance;
}

void Text_to_Binary_File(char *input_file, char *output_file)
{
    FILE *i = fopen(input_file, "r");
    FILE *o = fopen(output_file, "wb");

    while (!feof(i)) 
    {
        int key;
        char balance;
        if (fscanf(i, "%d %c\n", &key, &balance) != 2) {
            return ;
        };
        fwrite(&key, sizeof(key), 1, o);
        fwrite(&balance, sizeof(balance), 1, o);
    }
    fclose(i);
    fclose(o);
}
void Binary_to_Text_File(char *input_file, char *output_file)
{
    FILE *i = fopen(input_file, "rb");
    FILE *o = fopen(output_file, "w");
    int *open_success;
    int temp = 1;
    open_success = &temp;

    while (!feof(i)) 
    {
        int key = Binary_to_txt_key(i, open_success);
        char balance = Binary_to_txt_balance(i, open_success);
        if (feof(i)) break;
        fprintf(o, "%d %d\n", key, balance);
    }
    fclose(i);
    fclose(o);
}

Tnode *Build_BST_e(FILE *fp, int *open_success)
{
    int key = Binary_to_txt_key(fp, open_success);
    char balance = Binary_to_txt_balance(fp, open_success);

    Tnode *root = calloc(1, sizeof(Tnode));
    if (root == NULL) {
        fprintf(stderr, "[ERROR] malloc failed.");
        return NULL;
    }
    if (*open_success != 0) {
        root->key = key;
        if (balance == 0) 
            root->balance = 0;
        else if (balance == 1) {
            root->right = Build_BST_e(fp, open_success);
            // root->balance = root->right->balance + 1;
            // printf("root->key: %d, root->balance: %d\n",root->key, root->balance);
        }
        else if (balance == 2) {
            root->left = Build_BST_e(fp, open_success);
            // root->balance = root->left->balance - 1;
            // printf("root->key: %d, root->balance: %d\n",root->key, root->balance);
        }
        else if (balance == 3) {
            root->left = Build_BST_e(fp, open_success);
            root->right = Build_BST_e(fp, open_success);
        }
    }
    return root;
}

int Check_BST(Tnode *root, int min, int max)
{
    if (root == NULL)
        return 1;
    if (root->key < min || root->key > max)
        return 0;
    return Check_BST(root->left, min, root->key - 1) && Check_BST(root->right, root->key + 1, max);
}

int Check_Height_balanced(Tnode *root) 
{
    if (root == NULL) {
        return 0;
    }
    int left_h = Check_Height_balanced(root->left);
    // printf("root->key: %d, left_h: %d\n",root->key, left_h);
    if (left_h == -1) {
        return -1;
    }
    int right_h = Check_Height_balanced(root->right);
    // printf("root->key: %d, right_h: %d\n",root->key, right_h);
    if (right_h == -1) {
        return -1;
    }
    // printf("root->key: %d, left_h: %d, right_h: %d\n",root->key, left_h, right_h);
    if (left_h - right_h > 1 || left_h - right_h < -1) {
        return -1;
    }

    int max_h = (left_h > right_h) ? left_h : right_h;
    // printf("root->key: %d, max_h: %d\n",root->key, max_h);
    return 1 + max_h;
}

void Free_Tree(Tnode* node)
{
    if (node == NULL)
        return ;
    Free_Tree(node->left);
    Free_Tree(node->right);
    free(node);
}
