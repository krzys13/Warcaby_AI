#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
//#include"game_engine.h"

typedef struct link
{
    ruch data;
    struct link *next, *previous;
} link;

link* create_first(ruch first_data)
{
    link *first = malloc(sizeof(link));
    *first = (link) {.data = first_data, .previous = NULL, .next = NULL};
    return first;
}

void add(link *head, ruch new_data)
{
    
    while(head->next != NULL)
        head = head->next;
    link *new = malloc(sizeof(link));
    *new = (link) {.data = new_data, .previous = head, .next = NULL};
    head->next = new;
    
}

link *element_index(link *head, int index)
{
    for (int i = 0; i < index; i++)
    {
        head = head->next;
    }
    return head;
}

link* remove_link(link *head, int index)
{
    link *removed = element_index(head, index);
    
    if (removed->previous == NULL)
    {
        link *right = removed->next;
        right->previous = NULL;
        free(removed);
        head = right;
        return head;
    }
    else
    if (removed->next == NULL)
        {
            link *left = removed->previous;
            left->next = NULL;
            free(removed);
            return head;
        }
    else
        {
            link *left = removed->previous;
            link *right = removed->next;
            left->next = right;
            right->previous = left;
            free(removed);
            return head;
        }
}

void swap(link *head, int index1, int index2)
{
    link *remember_first = head;

    for (int i = 0; i < index1; i++)
    {
        head = head->next;
    }

    ruch temp_data1 = head->data;
    link *remember1 = head;
    head = remember_first;

    for (int i = 0; i < index2; i++)
    {
        head = head->next;
    }

    ruch temp_data2 = head->data;
    head->data = temp_data1;
    head = remember1;
    head->data = temp_data2;
}

link* insert_before(link *head, int index, ruch inserted_data)
{
    link *inserted_link = malloc(sizeof(link));
    *inserted_link = (link) {.data = inserted_data, .next = NULL, .previous = NULL};

    link *before_what = element_index(head, index);
    if (before_what->previous == NULL)
    {
        inserted_link->next = before_what;
        before_what->previous = inserted_link;
        return inserted_link;
    }
    else
    {
        before_what->previous->next = inserted_link;
        before_what->previous = inserted_link;
        inserted_link->previous = before_what->previous;
        inserted_link->next = before_what;
        return head;
    }
}

link* insert_after(link *head, int index, ruch inserted_data)
{
    link *inserted_link = malloc(sizeof(link));
    *inserted_link = (link) {.data = inserted_data, .next = NULL, .previous = NULL};

    link *after_what = element_index(head, index);
    if (after_what->next == NULL)
    {
        after_what->next = inserted_link;
        inserted_link->previous = after_what;
        return head;
    }
    else
    {
        inserted_link->next = after_what->next;
        after_what->next->previous = inserted_link;
        after_what->next = inserted_link;
        inserted_link->previous = after_what;
        return head;
    }
}

void destroy(link *head)
{
    if (head->next == NULL)
        free (head);
    else
    {
       destroy (head->next);
    }
}

int length(link *head)
{
    int list_length = 1;
    while (head->next != NULL)
    {
        head = head->next;
        list_length++;
    }
    return list_length;
}