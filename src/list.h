#ifndef _list_h
#define _list_h

/* A basic list implementation to help make my life easier. Adapted from
 * https://gist.github.com/pseudomuto/6334796
 */

typedef void (*freeFunction)(void *);

typedef struct _Node{
    void *data;
    struct _Node *next;
} _Node;

typedef struct List{
    _Node *head;
    _Node *tail;
    freeFunction freeFunction;
} List;

void List_append(List *list, void *data);
void List_prepend(List *list, void *data);
void *List_head(List *list);
void *List_tail(List *list);
void *List_pop(List *list);
void List_free(List *list);
List *List_new(freeFunction freeFunction);

#endif
