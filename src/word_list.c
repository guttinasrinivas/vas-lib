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

/*
 * TODO:
 *  Clean up memory mapping and close fd.
 */

int wl_read_list(word_list_t *wl)
{
    int ret = 0;
    int ii = 0;
    char *words = NULL;
    int tw = 0;
    
    ret = wl_map_file(wl);
    ReturnOnError(ret);
    words = wl->map;
    
    /* Split the buffer into words */
    tw = 0;
    for (ii = 0; ii < (int) wl->flen; ii++)
    {
        if (words[ii] == '\n')
        {
            words[ii] = '\0';
            tw++;
        }
    }
    wl->n_words = tw;
    LOG_Printf(INFO, "Found %d words in dictionary %s.\n", tw, wl->fname);
    wl->words = (char **)malloc(sizeof(char *) * tw);
    
    /* Split the words into array of strings */
    tw = 1;
    wl->words[0] = words;
    for (ii = 0; ii < (int) (wl->flen - 1); ii++)
    {
        if (words[ii] == '\0')
        {
            wl->words[tw] = (words + ii + 1);
            tw++;
        }
    }
    
    return(ret);
}


int wl_map_file(word_list_t *wl)
{
    int ret = 0;
    struct stat sb;
    
    LOG_Printf(INFO, "Attempting to open dictionary file %s...", wl->fname);
    wl->fd = open(wl->fname, O_RDONLY);
    if (wl->fd <= 0)
    {
        E_Printf("Could not open dictionary %s: error %d.\n", wl->fname, errno);
        return(errno);
    }
    
    ret = fstat(wl->fd, &sb);
    if (ret != 0)
    {
        E_Printf("%s: Access error.\n", wl->fname);
        return(errno);
    }
    
    wl->flen = sb.st_size;
    
    wl->map = (void *)mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE,
                         MAP_FILE|MAP_PRIVATE, wl->fd, 0);
    if (wl->map == MAP_FAILED)
    {
        E_Printf("Could not access dictionary file.\n");
        return(ENOMEM);
    }
    
    return(ret);
}


int wl_cleanup(word_list_t *wl)
{
    int ret = SUCCESS;
    
    ret = munmap(wl->map, wl->flen);
    wl->flen = 0;
    wl->map = NULL;
    free(wl->words);
    wl->words = NULL;
    close(wl->fd);
    
    return(ret);
}

/* TODO Find a better look up algorithm.
 * Maybe, hash table???
 */
int wl_lookup(const word_list_t *wl, const char *word, int *pos, uint8_t *valid_bm)
{
    int ret = SUCCESS;
    int ii =0;
    
    for(ii = 0; ii < wl->n_words; ii++)
    {
        if (bit_val_at(valid_bm, ii) == 0)
        {
            continue;
        }

        if (strcasecmp(word, wl->words[ii]) == 0)
        {
            //printf("Debug: %s == %s\n", word, wl->words[ii]);
            *pos = ii;
            return(ret);
        }
    }
    
    *pos = -1;
    ret = ENOENT;
    return(ret);
}


/* End of file */

