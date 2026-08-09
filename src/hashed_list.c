#include <stdlib.h>
#include "hashed_list.h"


int vl_hl_init(vl_hl_t *inlist)
{
    int ii = 0;
    inlist->ref.ct++;

    for (ii = 0; ii < HASHED_LIST_LEVEL_SIZE; ii++) {
        inlist->lvl_1[ii] = NULL;
    }

    return SUCCESS;
}


int vl_hl_insert(vl_hl_t *inlist, void *inbuf, size_t inlen)
{
    int ret = SUCCESS;
    vl_hl_hash_t hash;
    
    ret = vl_hl_compute_hash(inbuf, inlen, &hash);
    ReturnOnError(ret);

    vl_hl_tbl_page_t* lvl2 = (vl_hl_tbl_page_t*) inlist->lvl_1[hash.b.lvl1];
    if (lvl2 == NULL) {
        inlist->lvl_1[hash.b.lvl1] = vl_alloc(sizeof(vl_hl_tbl_page_t));
        lvl2 = (vl_hl_tbl_page_t*) inlist->lvl_1[hash.b.lvl1];
        for (int ii = 0; ii < HASHED_LIST_LEVEL_SIZE; ii++) {
            q_init(lvl2[ii]);
        }
    }

    q_node_t *nn;
    q_init_node(&nn, inbuf);
    q_append(lvl2[hash.b.lvl2], nn);

    return ret;
}


int vl_hl_compute_hash(uint8_t *inbuf, size_t inlen, vl_hl_hash_t *ohash)
{
    int ii = 0;
    ohash->dw0 = 0;
    int dwlen = (int) (inlen & (~0x1));

    for (ii = 0; ii < (int) dwlen; ii++) {
        ohash->w[0] += (inbuf[ii] | (inbuf[ii+1] << 8));
    }

    for (ii = 0; ii < ((int) inlen - dwlen); ii++) {
        ohash += (inbuf[dwlen + ii] << ii);
    }

    return SUCCESS;
}


int vl_hl_cleanup(vl_hl_t *inlist)
{
    /* TODO Implement BFS clean up */
    inlist->ref.ct--;

    return SUCCESS;
}

/* End of file */

