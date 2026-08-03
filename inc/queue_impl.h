#ifndef __QUEUE_IMPL_H__
#define __QUEUE_IMPL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct q_node_s {
    struct q_node_s* next;
    struct q_node_s* prev;
    struct q_node_s* parent;
    struct q_node_s* children;
    void* q;
    void* data;
} q_node_t;

typedef struct q_s {
    struct q_node_s* head;
    struct q_node_s* tail;
    int len;
    int debug;
} q_t;

int q_init_node(q_node_t** n, void* data);
int q_free_node(q_node_t** n);

int q_init(q_t *q);
int q_append(q_t *q, q_node_t* node);
int q_insert(q_t *q, q_node_t* at, q_node_t* node);
int q_delete(q_t *q, q_node_t* node);

int q_fifo_in(q_t *q, q_node_t* node);
int q_fifo_out(q_t *q, q_node_t** node);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __QUEUE_IMPL_H__ */
