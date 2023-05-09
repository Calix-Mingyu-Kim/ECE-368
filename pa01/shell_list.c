#include <stdio.h>
#include <stdlib.h>

#include "sequence.h"
#include "shell_list.h"

Node *List_Load_From_File(char *filename)
{
    FILE *fp = fopen(filename, "rb");

    //fail to open file
    if (fp == NULL){
        //return NULL address and assign 0 to *size
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return NULL;
    }

    long val;
    Node *head = NULL;
    Node *tail = NULL;    
    while (fread(&val, sizeof(long), 1, fp))
    {
        Node *newNode = (Node *)malloc(sizeof(Node));
        if(newNode == NULL){
            fprintf(stderr, "Unable to allocate memory for new node\n");
            return 0;
        }
        newNode->value = val;
        newNode->next = NULL;
        if (head == NULL) 
            head = newNode;
        else
            tail->next = newNode;
        tail = newNode;
    }

    fclose(fp);
    
    return head;
}

int List_Save_To_File(char *filename, Node *list)
{
    FILE *fp = fopen(filename, "wb");

    //fail to open file
    if (fp == NULL){
        //return NULL address and assign 0 to *size
        fprintf(stderr, "[ERROR] fopen failed.\n");
        return EXIT_FAILURE;
    }

    //If array is NULL or size is 0
    if (list == 0)
    {
        fclose(fp);
        return 0;
    }
    int saved = 0;

    while (list!=NULL) 
    {
        fwrite(list, sizeof(long), 1, fp);
        list = list->next;
        saved++;
    }

    fclose(fp);
    return saved;
}

Node *List_Shellsort(Node *list, long *n_comp)
{
    // Generate sequence for shell sort
    int seq_size;
    Node *cur = list;
    int length = 0;
    while(cur != NULL)
    {
        cur = cur->next;
        length++;
    }

    long *sequence = Generate_2p3q_Seq(length, &seq_size);
    Node *head = list;
    

    // Shell sort using bubble sort
    for (int x=seq_size-1; x>=0; x--)
    {
        Node *currX = head, *currY = head;
        Node *prevX = NULL, *prevY = NULL;
        
        int last_exchange = length;
        int k = (int)sequence[x];
        int count = k;

        while (count--)
        {
            prevY = currY;
            currY = currY->next;
        }

        int sorted = 0; // not sorted

        while (!sorted)
        {
            sorted = 1;
            int last_element = last_exchange - 1;
            for (int i=k; i<=last_element; i++)
            {
                int swapped = 0;
                *n_comp += 1;
                if (currX->value > currY->value)
                {
                    // swap two nodes
                    if (prevX != NULL)
                        prevX->next = currY;
                    else    //if head
                        head = currY;
                    if (prevY != NULL)
                        prevY->next = currX;
                    else    //if head
                        head = currX; 

                    Node* temp = currY->next;
                    currY->next = currX->next;
                    currX->next = temp;
                    last_exchange = i;
                    swapped = 1;
                }

                if (swapped)
                {
                    prevX = currY;
                    Node *currX_temp = currX;
                    currX = currY->next;
                    prevY = currX_temp;
                    currY = currX_temp->next;
                }
                else
                {
                    prevX = currX;
                    currX = currX->next;
                    prevY = currY;
                    currY = currY->next;
                }
            }
        }
    }

    free(sequence);

    return head;
}
