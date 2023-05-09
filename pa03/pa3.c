#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

int main(int argc, char *argv[])
{
    if (argc != 5){
        fprintf(stderr, "argc not 5");
        return EXIT_FAILURE;
    }
    int success = 0;

    // out_file1
    int activateXY = 0;
    Tnode *root1 = Tree_Load_From_File(argv[1]);
    Tnode *reRoot1 = reRootLR_test(root1);
    success += Preorder_Save_To_File(argv[2], reRoot1, activateXY);
    if (success < 0)
        return EXIT_FAILURE;

    // out_file2
    Tnode *root2 = Tree_Load_From_File(argv[1]);
    Tnode *reRoot2 = reRootRL_test(root2);
    success += Preorder_Save_To_File(argv[3], reRoot2, activateXY);
    if (success < 0)
        return EXIT_FAILURE;

    // out_file 3
    activateXY = 1;
    Tnode *root3 = Tree_Load_From_File(argv[1]);
    Tnode *reRoot3 = reRootAll(root3);
    success += Preorder_Save_To_File(argv[4], reRoot3, activateXY);
    if (success < 0)
        return EXIT_FAILURE;

    Free_Tree(reRoot1);
    Free_Tree(reRoot2);
    Free_Tree(reRoot3);
    
    return EXIT_SUCCESS;
}
