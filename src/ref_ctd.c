#include <stdlib.h>
#include "generics.h"
#include "simple_logger.h"


inline __attribute__((always_inline)) void *vl_alloc(size_t inlen)
{
    void *obuf = malloc(inlen);
    if (obuf == NULL) {
        return NULL;
    }

    memset(obuf, 0, inlen);
    ((RefCtdObj_t *) obuf)->ref.marker = REF_CTR_MARKER;
    ((RefCtdObj_t *) obuf)->ref.blen = inlen;

    return obuf;
}


void ref_ct_dwn(RefCtdObj_t** bufp)
{
    int blen = 0;
    AssertMarker(*bufp);

    LOG_Printf(DEBUG, "Ref count down: (0x%llx)", (unsigned long long) *bufp);

    /* This is a bit tricky.
     * The Ref Count maybe zero for a free'd block. */
    if ((*bufp)->ref.ct <= 0) {
        LOG_Printf(ERROR, "Reference count for *0x%llx is less than 0",
                   (uint64_t) *bufp);
        exit(100);
    }
    (*bufp)->ref.ct --;

    if ((*bufp)->ref.ct != 0) {
        return;
    }

    blen = (int) (*bufp)->ref.blen;
    if ((blen <= 0) ||
        (blen >= REF_CTR_MAX_MANAGED_LENG))
    {
        LOG_Printf(ERROR, "Invalid buffer length %d for *0x%llx",
                   (int) (*bufp)->ref.blen,
                   (uint64_t) *bufp);
        exit(101);
        return;
    }

    LOG_Printf(DEBUG, "Freeing word_list_t *0x%llx", (uint64_t) *bufp);
    memset((void*)(*bufp), 0, (*bufp)->ref.blen);
    free((void*)(*bufp));

    return;
}


