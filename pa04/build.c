#include <stdio.h>
#include <stdlib.h>

#include "hbt.h"
#include "build.h"
#include "eval.h"

Tnode *Build_BST_b(FILE *fp, int *open_success)
{
    Tnode *tmp_root = NULL;
    Tnode **root = &tmp_root;
    // Tnode **root = calloc(1, sizeof(Tnode));
    // if (root == NULL) {
    //     fprintf(stderr, "[ERROR] malloc failed.");
    //     return NULL;
    // }
    while (*open_success != 0) {
        int key = Binary_to_txt_key(fp, open_success);
        char balance = Binary_to_txt_balance(fp, open_success);
        if (feof(fp)) break;
        if (balance == 'i') {
            Search_and_insert(root, key);
        }
        else if (balance == 'd') {
            Search_and_delete(root, key);
        }
    }

    return *root;
}

Tnode *Search_and_insert(Tnode **root, int key)
{
    Tnode *prev = NULL, *pya = NULL, *q = NULL;    //pya: parent of youngest ancestor
    Tnode *curr = *root, *ya = *root;   // ya: youngest ancestor with balance 1/-1

    while (curr != NULL) {
        if (key <= curr->key)
            q = curr->left;
        else
            q = curr->right;
        if (q != NULL && q->balance != 0) {
            pya = curr;
            ya = q;
        }
        prev = curr;
        curr = q;
    }

    Tnode *node = calloc(1, sizeof(Tnode));
    if (node == NULL) {
        fprintf(stderr, "[ERROR] failed to allocate sufficient memory");
        return NULL;
    }

    node->key = key;
    node->balance = 0;
    if (prev == NULL)
        *root = node;
    else {
        if (key <= prev->key)
            prev->left = node;
        else
            prev->right = node;
        curr = ya;
        // update the balance from youngest ancestor to parent of the new node
        while (curr != node) {
            if (key <= curr->key) {
                curr->balance++;
                curr = curr->left;
            }
            else {
                curr->balance--;
                curr = curr->right;
            }
        }
        // check if balance is required
        
        if ((ya->balance < 2) && (ya->balance > -2))
            return node;
        Tnode *child;
        if (key <= ya->key)
            child = ya->left;
        else
            child = ya->right;

        if ((ya->balance == 2) && (child->balance == 1)) {
            curr = CR(ya);  // clockwise rotation
            ya->balance = 0;
            child->balance = 0;
        }
        if ((ya->balance == -2) && (child->balance == -1)) {
            curr = CCR(ya);
            ya->balance = 0;
            child->balance = 0;
        }
        //ya and child are unbalanced in opposite directions
        if ((ya->balance == 2) && (child->balance == -1)) {
            ya->left = CCR(child);
            curr = CR(ya);
            if (curr->balance == 0) {
                ya->balance = 0;
                child->balance = 0;
            }
            else {
                if (curr->balance == 1) {
                    ya->balance = -1;
                    child->balance = 0;
                }
                else {
                    ya->balance = 0;
                    child->balance = 1;
                }
                curr->balance = 0;
            }
        }
        if ((ya->balance == -2) && (child->balance == 1)) {
            ya->right = CR(child);
            curr = CCR(ya);
            if (curr->balance == 0) {
                ya->balance = 0;
                child->balance = 0;
            }
            else {
                if (curr->balance == -1) {
                    ya->balance = 1;
                    child->balance = 0;
                }
                else {
                    ya->balance = 0;
                    child->balance = -1;
                }
                curr->balance = 0;
            }
        }
        if (pya == NULL)
            *root = curr;
        else {
            if (key <= pya->key)
                pya->left = curr;
            else
                pya->right = curr;
        }
    }
    return node;
}

Tnode *CR(Tnode *old_root) 
{
    Tnode *new_root = old_root->left;
    old_root->left = new_root->right;
    new_root->right = old_root;
    return new_root;
}

Tnode *CCR(Tnode *old_root)
{
    Tnode *new_root = old_root->right;
    old_root->right = new_root->left;
    new_root->left = old_root;
    return new_root;
}

Tnode *Find_Predecessor(Tnode *curr, Tnode *prev, Tnode *p_predecessor, Tnode *predecessor, int *update_h)
{
    Tnode *node = NULL;
    if (!predecessor->right) {
        if (prev == NULL) { // root node to be deleted
            if (p_predecessor != curr) {
                p_predecessor->right = predecessor->left; 
                predecessor->left = curr->left;
                predecessor->right = curr->right;
                predecessor->balance = curr->balance;
                node = p_predecessor; //start balancing from predecessor
                node->balance++;
            }
            else {
                predecessor->right = curr->right;
                node = predecessor;
                predecessor->balance = curr->balance-1;
            }
        }
        else {  // non root node to be deleted
            if (p_predecessor != curr) {
                p_predecessor->right = predecessor->left; 
                predecessor->left = curr->left;
                predecessor->balance = curr->balance;
                
                node = p_predecessor; //start balancing from p_predecessor
                node->balance++;
            }
            else {
                node = predecessor;//start balancing from predecessor
                predecessor->balance = curr->balance-1;
            }
            predecessor->right = curr->right;
            if (node->balance > 1 || node->balance < -1)
                predecessor = Check_Balance(node);
            if (prev->key > curr->key) {
                prev->left = predecessor;
            }
            else {
                prev->right = predecessor;
            }
        }
        
        if (node->balance == 0)
            *update_h = 1;
        free(curr);
        
        return predecessor;   
    }
    else
        node = Find_Predecessor(curr, prev, predecessor, predecessor->right, update_h);
    
    //do something
    // Check_Balance()
    if (*update_h) {
        if (predecessor->balance != 0) {
            *update_h = 0;
        }
        if (p_predecessor == curr) {
            node->balance--;
        }
        else
            p_predecessor->balance++;
    }
    
    if (p_predecessor == curr) {
        if (node->balance > 1 || node->balance < -1)
            Check_Balance(node);
    }
    else {
        if (p_predecessor->balance > 1 || p_predecessor->balance < -1)
            Check_Balance(p_predecessor);
    }
    
    return node;
}

Tnode *Check_Balance(Tnode *parent) 
{
    Tnode *curr = NULL, *child = NULL;
     if (parent->balance == 2)
        child = parent->left;
    else
        child = parent->right;

    if ((parent->balance == 2) && (child->balance == 1)) {
        curr = CR(parent);  // clockwise rotation
        parent->balance = 0;
        child->balance = 0;
    }
    if ((parent->balance == -2) && (child->balance == -1)) {
        curr = CCR(parent);
        parent->balance = 0;
        child->balance = 0;
    }
    //parent and child are unbalanced in opposite directions
    if ((parent->balance == 2) && (child->balance == -1)) {
        parent->left = CCR(child);
        curr = CR(parent);
        if (curr->balance == 0) {
            parent->balance = 0;
            child->balance = 0;
        }
        else {
            if (curr->balance == 1) {
                parent->balance = -1;
                child->balance = 0;
            }
            else {
                parent->balance = 0;
                child->balance = 1;
            }
            curr->balance = 0;
        }
    }
    if ((parent->balance == -2) && (child->balance == 1)) {
        parent->right = CR(child);
        curr = CCR(parent);
        if (curr->balance == 0) {
            parent->balance = 0;
            child->balance = 0;
        }
        else {
            if (curr->balance == -1) {
                parent->balance = 1;
                child->balance = 0;
            }
            else {
                parent->balance = 0;
                child->balance = -1;
            }
            curr->balance = 0;
        }
    }
    if ((parent->balance == 2) && (child->balance == 0)) {
        curr = CR(parent);  // clockwise rotation
        parent->balance = -1;
        child->balance = -1;
    }
    if ((parent->balance == -2) && (child->balance == 0)) {
        curr = CCR(parent);
        parent->balance = -1;
        child->balance = 1;
    }
    return curr;
}

Tnode *Delete_and_balance(Tnode *curr, Tnode *prev, int key, int *update_h)
{
    Tnode *node = NULL;
    if (key == curr->key) {
        // case 3: node has two subtrees
        if (curr->left && curr->right) {    //two subtrees
            //find immediate predecessor
            Tnode *p_predecessor = curr;    //parent of predecessor
            Tnode *predecessor = curr->left;
            node = Find_Predecessor(curr, prev, p_predecessor, predecessor, update_h);
            // if ()
        }
        // case 2: node has one subtree
        else {
            if (curr->left) {              //one subtree to left
                if (prev->key > curr->key) {
                    prev->left = curr->left;
                    prev->balance--;
                }
                else {
                    prev->right = curr->left;   //update balance of prev
                    prev->balance++;
                }
            }
            else if (curr->right) {             //one subtree to right
                if (prev->key > curr->key) {
                    prev->left = curr->right;
                    prev->balance--;
                }
                else {
                    prev->right = curr->right;  //update balance of prev
                    prev->balance++;
                }
            }
            // case 1: node does not have child node
            else {
                if (prev != NULL) { // is not root    //send to bottom
                    if (prev->key > curr->key) {
                        prev->left = NULL;
                        prev->balance--;
                    }
                    else {
                        prev->right = NULL;
                        prev->balance++;
                    }
                }
            }
            if (prev != NULL) {
                node = prev;
            }
            // curr->left = NULL;
            // curr->right = NULL;
            // curr->key = NULL;
            // curr->balance = NULL;
            free(curr); //includes prev == NULL (curr is root node)  
        }
        if (node->balance == 0) 
            *update_h = 1;
        if (node->balance > 1 || node->balance < -1) {    //check for last 1 d in ops3.b
            prev = Check_Balance(node);
            if (node->balance == 0) 
                *update_h = 1;
        } 
        if (prev != NULL) {
            node = prev;
        }
       
        
        // check if balance is required
        // if (node->balance > 1 || node->balance < -1) {
        //     node = Check_Balance(node);
        // } 
        // if (node->balance == 0) 
        //     *update_h = 1;

        return node;
    }
    if (key < curr->key)
        node = Delete_and_balance(curr->left, curr, key, update_h); //node is prev
    else {
        // printf("update b4: %d, %d\n", node, node->key);
        node = Delete_and_balance(curr->right, curr, key, update_h);
    }
    
    // after node is deleted, balanced
    if (node->balance > 1 || node->balance < -1) {    //check for last 1 d in ops3.b
        node = Check_Balance(node);
        if (node->balance == 0) 
            *update_h = 1;
    } 

    if (prev == NULL) {
        return node;
    }
    else {
        if (node->key < prev->key && prev->left != node) 
            prev->left = node;
        else if (node->key >= prev->key && prev->right != node)
            prev->right = node;
    }

    if (*update_h) {
        if (node->balance != 0) {
            *update_h = 0;
        }
        if (prev != NULL) {
            if (node->key < prev->key)
                prev->balance--;
            else 
                prev->balance++;
        }
    }

    return prev;
}

Tnode *Search_and_delete(Tnode **root, int key)
{
    Tnode *q = NULL;    
    Tnode *curr = *root;
    while (curr != NULL) {
        if (key == curr->key) 
            break;
        if (key < curr->key)
            q = curr->left;
        else
            q = curr->right;

        // prev = curr;
        curr = q;
    }
    if (curr == NULL)   //key not found
        return *root;

    int update_h = 0;
    *root = Delete_and_balance(*root, NULL, key, &update_h);

    return *root;
}

void Preorder_BST(FILE *fp, Tnode *root)
{
    if (root == NULL)
        return ;

    int key = root->key;
    char balance = 0;
    if (root->left && root->right)
        balance = 3;
    else if (root->left && !root->right)
        balance = 2;
    else if (root->right)
        balance = 1;
    
    fwrite(&key, sizeof(key), 1, fp);
    fwrite(&balance, sizeof(balance), 1, fp);
    Preorder_BST(fp, root->left);
    Preorder_BST(fp, root->right);
}
