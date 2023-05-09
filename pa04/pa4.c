#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hbt.h"
#include "build.h"
#include "eval.h"

int main(int argc, char *argv[])
{
    // Text_to_Binary_File("Test.txt", "TEST_DUP.b");
    if (strcmp(argv[1], "-e") == 0){
        if (argc != 3){
            fprintf(stderr, "[ERROR] argc not 3");
            return EXIT_FAILURE;
        }
        int *open_success;
        int temp = 1;
        open_success = &temp;
        int BST = 1;
        int height_balanced = 0;

        FILE *fp = fopen(argv[2], "rb");
        if (fp == NULL) {
            fprintf(stderr, "[ERROR] fopen failed.\n");
            *open_success = -1;
        }
        Tnode *root = NULL;
        if (*open_success > 0)
            root = Build_BST_e(fp, open_success);
        if (*open_success > 0) {
            BST = Check_BST(root, HBT_MIN, HBT_MAX);
            height_balanced = Check_Height_balanced(root);
            height_balanced = (height_balanced == -1) ? 0 : 1;
            fclose(fp);
        }
        Free_Tree(root);
        fprintf(stdout, "%d,%d,%d\n", *open_success, BST, height_balanced);

        if (*open_success < 1)
            return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }
    else if (strcmp(argv[1], "-b") == 0){
        int *open_success;
        int temp = 1;
        open_success = &temp;

        if (argc != 4){
            fprintf(stderr, "[ERROR] argc not 4");
            return EXIT_FAILURE;
        }
        FILE *fp = fopen(argv[2], "rb");

        // If input file cannot be opened
        if (fp == NULL) {
            *open_success = -1;
            fprintf(stdout,"%d\n", *open_success);
            return EXIT_FAILURE;
        }
        Tnode *root = Build_BST_b(fp, open_success);
        fclose(fp);
        
        //Preorder_save_to_file
        FILE *o = fopen(argv[3], "wb");   
        //fail to open file
        if (o == NULL){
            fprintf(stderr, "[ERROR] fopen failed. for save\n");
            fprintf(stdout,"%d\n", *open_success);
            return EXIT_FAILURE;
        }
        Preorder_BST(o, root);
        fclose(o);
        Free_Tree(root);

        // If encounters problem in input file
        if (*open_success == 0) {
            fprintf(stdout,"%d\n", *open_success);
            return EXIT_FAILURE;
        }
        // Binary_to_Text_File(argv[3], "OUTPUT.txt"); //TEST MUST DELETE!!!!!!!

        fprintf(stdout, "%d\n", 1);
        return EXIT_SUCCESS;

    }
    return EXIT_FAILURE;
}