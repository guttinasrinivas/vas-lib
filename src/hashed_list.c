#include <stdlib.h>
#include <stdio.h>
#include "hashed_list.h"
#include "generics.h"


int vl_hl_init(vl_hl_t* inlist)
{
    int ii = 0;
    inlist->ref.ct++;

    for (ii = 0; ii < HASHED_LIST_LEVEL_SIZE; ii++) {
        inlist->lvl_1.dirs[ii] = NULL;
    }

    return SUCCESS;
}


int vl_hl_insert(vl_hl_t* inlist, void* inbuf, size_t inlen)
{
    int ret = SUCCESS;
    vl_hl_hash_t hash;

    ret = vl_hl_compute_hash(inbuf, inlen, &hash);
    ReturnOnError(ret);

    vl_hl_tbl_page_t* lvl2 = inlist->lvl_1.dirs[hash.b.lvl1];
    if (lvl2 == NULL) {
        inlist->lvl_1.dirs[hash.b.lvl1] = vl_alloc(sizeof(vl_hl_tbl_page_t));
        lvl2 = inlist->lvl_1.dirs[hash.b.lvl1];
        for (int ii = 0; ii < HASHED_LIST_LEVEL_SIZE; ii++) {
            if (lvl2->pages[ii] == NULL) {
                lvl2->pages[ii] = vl_alloc(sizeof(q_t));
            }
            q_t *qaddr = lvl2->pages[ii];
            q_init(qaddr);
        }
    }

    inlist->ct++;
    vl_hl_entry_t* ent = vl_alloc(sizeof(vl_hl_entry_t));
    ent->data = inbuf;
    ent->dlen = inlen;
    ent->idx = inlist->ct;

    q_node_t* nn;
    q_init_node(&nn, ent);
    q_append(lvl2->pages[hash.b.lvl2], nn);

    return ret;
}


int vl_hl_lookup(vl_hl_t* inlist, void* inbuf, int inlen, int* idx)
{
    int ret = 0;
    vl_hl_hash_t hash;

    ret = vl_hl_compute_hash(inbuf, inlen, &hash);
    ReturnOnError(ret);

    vl_hl_tbl_page_t* lvl2 = inlist->lvl_1.dirs[hash.b.lvl1];
    if (lvl2 == NULL) {
        return ENOENT;
    }

    q_node_t* nn;
    vl_hl_entry_t* ent;
    q_t *qaddr = lvl2->pages[hash.b.lvl2];
    nn = qaddr->head;
    for (int ii = 0; ii < qaddr->len; ii++) {
        if (nn == NULL) {
            return ENOENT;
        }

        ent = nn->data;
        if (ent->dlen != inlen) {
            nn = nn->next;
            continue;
        }

        if (memcmp(ent->data, inbuf, inlen) == 0) {
            *idx = ent->idx;
            return SUCCESS;
        }

        nn = nn->next;
    }

    return ENOENT;
}


int vl_hl_print(vl_hl_t* inlist)
{
    int ii = 0;
    int jj = 0;
    int total_words = 0;

    for (ii = 0; ii < HASHED_LIST_LEVEL_SIZE; ii++) {
        if (inlist->lvl_1.dirs[ii] == NULL) {
            continue;
        }

        vl_hl_tbl_page_t* tbl = inlist->lvl_1.dirs[ii];
        /* Iterate over the lvl_2 and print the entry from each queue */
        for (jj = 0; jj < HASHED_LIST_LEVEL_SIZE; jj++) {
            q_t *currq = tbl->pages[jj];
            q_node_t* currn = currq->head;
            if (currn == NULL) {
                printf("inlist->lvl_1[%d]->lvl_2[%d] == NULL\n", ii, jj);
                continue;
            }

            printf("[%d][%d]: %d\n", ii, jj, currq->len);
            total_words += currq->len;
        }
    }

    printf("Total words from hash table: %d\n", total_words);

    return SUCCESS;
}


int vl_hl_compute_hash(uint8_t* inbuf, size_t inlen, vl_hl_hash_t* ohash)
{
    int ii = 0;
    ohash->dw0 = 0;
    int dwlen = (int)(inlen & (~0x1));

    for (ii = 0; ii < (int) dwlen; ii++) {
        ohash->w[0] += (inbuf[ii] | (inbuf[ii + 1] << 8));
    }

    for (ii = 0; ii < ((int) inlen - dwlen); ii++) {
        ohash += (inbuf[dwlen + ii] << ii);
    }

    return SUCCESS;
}


int vl_hl_cleanup(vl_hl_t* inlist)
{
    /* TODO Implement BFS clean up */
    inlist->ref.ct--;

    return SUCCESS;
}

/* End of file */

