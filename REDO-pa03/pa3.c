#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

int main(int argc, char *argv[])
{
    if (argc != 5){
        fprintf(stderr, "argc not 5");
        return EXIT_FAILURE;
    }
    int activateXY = 0;
    // ----------------------------------------------------------------------- out_file1
    FILE *in_1 = fopen(argv[1], "r");
    //fail to open file
    if (in_1 == NULL){
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return EXIT_FAILURE;
    }
    fseek(in_1, -2, SEEK_END);    // very last element is always an alphabet.
    Tnode *root1 = Build_BST(in_1);
    fclose(in_1);

    Tnode *reRoot1 = reRootLR(root1);

    FILE *out_1 = fopen(argv[2], "w");
    //fail to open file
    if (out_1 == NULL){
        fprintf(stderr, "[ERROR] fopen failed. for save\n");
        return EXIT_FAILURE;
    }
    Preorder_BST(out_1, reRoot1, activateXY);
    fclose(out_1);
    // ----------------------------------------------------------------------- out_file2
    FILE *in_2 = fopen(argv[1], "r");
    //fail to open file
    if (in_2 == NULL){
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return EXIT_FAILURE;
    }
    fseek(in_2, -2, SEEK_END);    // very last element is always an alphabet.
    Tnode *root2 = Build_BST(in_2);
    fclose(in_2);

    Tnode *reRoot2 = reRootRL(root2);

    FILE *out_2 = fopen(argv[3], "w");
    //fail to open file
    if (out_2 == NULL){
        fprintf(stderr, "[ERROR] fopen failed. for save\n");
        return EXIT_FAILURE;
    }
    Preorder_BST(out_2, reRoot2, activateXY);
    fclose(out_2);
    // ----------------------------------------------------------------------- out_file3
    activateXY = 1;
    
    FILE *in_3 = fopen(argv[1], "r");
    //fail to open file
    if (in_3 == NULL){
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return EXIT_FAILURE;
    }
    fseek(in_3, -2, SEEK_END);    // very last element is always an alphabet.
    Tnode *root3 = Build_BST(in_3);
    fclose(in_3);

    Tnode *reRoot3 = reRootAll(root3);

    FILE *out_3 = fopen(argv[4], "w");
    //fail to open file
    if (out_3 == NULL){
        fprintf(stderr, "[ERROR] fopen failed. for save\n");
        return EXIT_FAILURE;
    }
    Preorder_BST(out_3, reRoot3, activateXY);
    fclose(out_3);

    Free_Tree(reRoot1);
    Free_Tree(reRoot2);
    Free_Tree(reRoot3);
    
    return EXIT_SUCCESS;
}