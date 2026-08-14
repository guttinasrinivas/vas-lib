#ifndef __HASHED_LIST_H__
#define __HASHED_LIST_H__

#include "generics.h"
#include "queue_impl.h"


#define HASHED_LIST_LEVEL_SIZE  (16)

typedef struct {
    RefCtd;
    void* data;
    int dlen;
    int idx;
} vl_hl_entry_t;

typedef struct {
    RefCtd;
    q_t* pages[HASHED_LIST_LEVEL_SIZE];
} vl_hl_tbl_page_t;

typedef struct {
    RefCtd;
    vl_hl_tbl_page_t* dirs[HASHED_LIST_LEVEL_SIZE];
} vl_hl_tbl_dir_t;


typedef union vl_hl_hash_s {
    struct {
        uint32_t lvl1: 4;
        uint32_t lvl2: 4;
        uint32_t lvl3: 8;
    } b;

    uint16_t w[2];
    uint32_t dw0;
} vl_hl_hash_t;


typedef struct vl_hl_s {
    RefCtd;
    vl_hl_tbl_dir_t lvl_1;
    int ct;
} vl_hl_t;


int vl_hl_init(vl_hl_t* inlist);
int vl_hl_compute_hash(uint8_t* inbuf, size_t inlen, vl_hl_hash_t* ohash);
int vl_hl_insert(vl_hl_t* inlist, void* inbuf, size_t inlen);
int vl_hl_lookup(vl_hl_t* inlist, void* inbuf, int inlen, int* idx);
int vl_hl_print(vl_hl_t* inlist);
int vl_hl_cleanup(vl_hl_t* inlist);

#endif /* __HASHED_LIST_H__ */

