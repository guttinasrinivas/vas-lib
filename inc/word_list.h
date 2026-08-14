#ifndef __WORD_LIST_H__
#define __WORD_LIST_H__
#include <stdint.h>
#include <memory.h>
#include "generics.h"
#include "hashed_list.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct word_list_s {
    RefCtd;
    char* fname;
    int fd;
    size_t flen;
    void* map;
    char* filters;
    char** words;
    int n_words;
    vl_hl_t* hl;
} word_list_t;

int wl_read_list(word_list_t* wl);
int wl_map_file(word_list_t* wl);
int wl_cleanup(word_list_t* wl);

int wl_lookup(const word_list_t* wl, const char* word, int* pos,
              uint8_t* valid_bm);
int wl_remove(const word_list_t* wl, int pos);

#ifdef __cplusplus
}
#endif

#endif /* __WORD_LIST_H__ */
