#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "generics.h"
#include "queue_impl.h"

/* Private symbols */
static void q_debug_print(q_t *q);

int q_init(q_t *q)
{
    int ret = SUCCESS;
    q->head = NULL;
    q->tail = NULL;
    q->len = 0;
    
    return(ret);
}

int q_append(q_t *q, q_node_t *node)
{
    q_node_t *head = q->head;
    
    if ((q == NULL) ||
        (node == NULL))
    {
        return ENOTSUP;
    }
    
    if (head == NULL)
    {
        node->next = NULL;
        node->prev = NULL;
        node->q = (void*) q;
        q->tail = node;
        q->tail = node;
        q->len = 1;
        return SUCCESS;
    }
    
    node->next = NULL;
    node->prev = q->tail;
    node->q = (void*) q;
    q->tail->next = node;
    q->tail = node;
    q->len++;
    
    return (SUCCESS);
}

int q_insert(q_t *q, q_node_t *at, q_node_t *node)
{
    q_node_t *next = NULL;
    
    if ((q == NULL) ||
        (at == NULL) ||
        (node == NULL))
    {
        return ENOTSUP;
    }
    
    next = at->next;
    if (next != NULL)
    {
        next->prev = node;
    }
    node->next = next;
    
    at->next = node;
    node->prev = at;
    node->q = (void *) q;
    q->len++;
    
    return(SUCCESS);
}

int q_delete(q_t *q, q_node_t *node)
{
    q_node_t *prev = NULL;
    q_node_t *next = NULL;
    
    if ((q == NULL) ||
        (node == NULL))
    {
        return(ENOENT);
    }
    
    prev = node->prev;
    next = node->next;
    
    if (prev != NULL)
    {
        prev->next = next;
    }
    
    if (next != NULL)
    {
        next->prev = prev;
    }
    q->len--;
    
    return(SUCCESS);
}

int q_fifo_in(q_t *q, q_node_t *node)
{
    int ret = SUCCESS;
    q_node_t *head;
    
    q_debug_print(q);
    if ((q == NULL) ||
        (node == NULL))
    {
        return(ENOENT);
    }
    
    head = q->head;
    
    node->next = head;
    node->prev = NULL;
    node->q = (void *) q;
    
    if(head != NULL)
    {
        head->prev = node;
    }
    else
    {
        q->tail = node;
    }
    
    q->head = node;
    q->len++;
    
    return(ret);
}
int q_fifo_out(q_t *q, q_node_t **node)
{
    int ret = SUCCESS;
    *node = NULL;
    
    q_debug_print(q);
    if (q == NULL)
    {
        return(ENOENT);
    }
    
    if ((q->tail == NULL) ||
        (q->len == 0))
    {
        return(ENOENT);
    }
    
    *node = q->tail;
    q->tail = (*node)->prev;
    q->len--;
    (*node)->prev = NULL;
    (*node)->q = NULL;
    
    if ((q->tail == NULL) ||
        (q->len == 0))
    {
        q->head = NULL;
    }
    
    return(ret);
}

int q_init_node(q_node_t **n, void* data)
{
    int ret = SUCCESS;
    
    /* XXX Check malloc return. */
    *n = malloc(sizeof(q_node_t));
    (*n)->data = data;
    (*n)->prev = NULL;
    (*n)->next = NULL;
    
    return(ret);
}

int q_free_node(q_node_t **n)
{
    int ret = SUCCESS;
    
    if ((*n)->data != NULL)
    {
        free((*n)->data);
    }
    
    free(*n);
    *n = NULL;
    
    return(ret);
}

static void q_debug_print(q_t *q)
{
/* #define Q_DEBUG_PRINT */
#ifdef Q_DEBUG_PRINT
    int ii = 0;
    q_node_t *node = NULL;
    LOG_PRINTF(INFO, "Queue of %d nodes at 0x%08lx: ", q->len, ((long) q));
    
    if (q == NULL)
    {
        LOG_PRINTF(INFO, "\n");
        return;
    }
    
    node = q->head;
    for (ii = 0; ii < q->len; ii++)
    {
        LOG_PRINTF(INFO, "<- (%d) [ p: 0x%08lx, q: 0x%08lx, d: 0x%08lx, n: 0x%08lx ] -> ",
                    ii,
                    (long) node->prev,
                    (long) node->q,
                    (long) node->data,
                    (long) node->next);
        if (node->next == NULL)
        {
            break;
        }
        node = node->next;
    }
    
    LOG_PRINTF("\n");
#endif /* Q_DEBUG_PRINT */
    return;
}
