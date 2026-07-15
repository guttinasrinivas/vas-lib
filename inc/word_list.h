#ifndef __WORD_LIST_H__
#define __WORD_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct word_list_s
{
    char *fname;
    int fd;
    size_t flen;
    void *map;
    char **words;
    int n_words;
} word_list_t;

int wl_read_list(word_list_t *wl);
int wl_map_file(word_list_t *wl);
int wl_cleanup(word_list_t *wl);

int wl_lookup(const word_list_t *wl, const char *word, int *pos);

#ifdef __cplusplus
}
#endif

#endif /* __WORD_LIST_H__ */
