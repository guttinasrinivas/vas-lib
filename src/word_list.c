#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "generics.h"
//#include "queue_impl.h"
#include "word_list.h"
#include "hashed_list.h"

/*
 * TODO:
 *  Clean up memory mapping and close fd.
 */

int wl_read_list(word_list_t* wl)
{
    int ret = 0;
    int ii = 0;
    char* words = NULL;
    int tw = 0;
    char *currword = NULL;

    wl->hl = vl_alloc(sizeof(vl_hl_t));
    ret = vl_hl_init(wl->hl);
    ReturnOnError(ret);

    ret = wl_map_file(wl);
    ReturnOnError(ret);
    words = wl->map;

    /* Split the buffer into words */
    tw = 0;
    for (ii = 0; ii < (int) wl->flen; ii++) {
        if (words[ii] == '\n') {
            words[ii] = '\0';
            tw++;
        }
    }
    wl->n_words = tw;
    wl->words = (char**)malloc(sizeof(char*) * tw);

    /* Split the words into array of strings */
    tw = 1;
    wl->words[0] = words;
    for (ii = 0; ii < (int)(wl->flen - 1); ii++) {
        if (words[ii] == '\0') {
            currword = (words + ii + 1);
            wl->words[tw] = currword;
            
            vl_hl_insert(wl->hl, currword, strlen(currword));
            tw++;
        }
    }

    printf("Total words loaded: %d\n", wl->n_words);
    
    return (ret);
}


int wl_map_file(word_list_t* wl)
{
    int ret = 0;
    struct stat sb;

    wl->fd = open(wl->fname, O_RDONLY);
    if (wl->fd <= 0) {
        return (errno);
    }

    ret = fstat(wl->fd, &sb);
    if (ret != 0) {
        return (errno);
    }

    wl->flen = sb.st_size;

#ifdef USE_MMAP_DICT
    wl->map = (void*)mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE,
                          MAP_FILE | MAP_PRIVATE, wl->fd, 0);
    if (wl->map == MAP_FAILED) {
        return (ENOMEM);
    }
#else /* USE_MMAP_DICT */
    wl->map = malloc(sb.st_size);
    if (wl->map == NULL) {
        return (ENOMEM);
    }

    int readlen = 0;
    while (readlen < sb.st_size) {
        int brdlen = read(wl->fd, wl->map, sb.st_size);
        if (brdlen < 0) {
            return ENOMEM;
        }
        readlen += brdlen;
    }
#endif /* USE_MMAP_DICT */

    return (ret);
}


int wl_cleanup(word_list_t* wl)
{
    int ret = SUCCESS;

#ifdef USE_MMAP_DICT
    ret = munmap(wl->map, wl->flen);
#else /* USE_MMAP_DICT */
    free(wl->map);
#endif /* USE_MMAP_DICT */
    wl->flen = 0;
    wl->map = NULL;
    free(wl->words);
    wl->words = NULL;
    close(wl->fd);

    return (ret);
}

/* TODO Find a better look up algorithm.
 * Maybe, hash table???
 */
int wl_lookup(const word_list_t* wl, const char* word, int* pos,
              uint8_t* valid_bm)
{
    int ret = SUCCESS;
    int explen = strlen(word);
    *pos = -1;

#ifdef SEQENTIAL_LOOK_UP
    for (int ii = 0; ii < wl->n_words; ii++) {
        if (bit_val_at(valid_bm, ii) == 0) {
            continue;
        }


        if (BUF_TO_DWORD(word, 0) == BUF_TO_DWORD(wl->words[ii], 0)) {
            *pos = ii;
            return (ret);
        }

        if (strncmp(word, wl->words[ii], explen) == 0) {
            *pos = ii;
            return (ret);
        }
    }
#else /* SEQUENTIAL_LOOK_UP */
    ret = vl_hl_lookup(wl->hl, (void*) word, explen, pos);
    ReturnOnError(ret);

    if (bit_val_at(valid_bm, *pos) == 0) {
        return ENOENT;
    }

#endif /* SEQUENTIAL_LOOK_UP */

    return ret;
}


int wl_remove(const word_list_t* wl, int pos)
{
    int ret = SUCCESS;
    char* word = NULL;

    if ((pos < 0) ||
            (pos >= wl->n_words)) {
        return (EINVAL);
    }

    word = wl->words[pos];
    word[0] = '\0';

    return (ret);
}

/* End of file */

