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
    Tnode *root = Tree_Load_From_File(argv[1]);
    success += Postorder_Save_To_File(argv[2], root);
    if (success < 0)
        return EXIT_FAILURE;

    // out_file2
    Postorder_rebuild_BST(root);
    success += Postorder_Save_To_File_2(argv[3], root);
    if (success < 0)
        return EXIT_FAILURE;

    // out_file 3
    Assign_Coord(root);
    success += Postorder_Save_To_File_3(argv[4], root);
    if (success < 0)
        return EXIT_FAILURE;

    Free_Tree(root);

    return EXIT_SUCCESS;
}