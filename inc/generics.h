#ifndef __GENERICS_H__
#define __GENERICS_H__

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Error codes */
enum {
    SUCCESS = (0),
    E_ARGS = (1),
    E_NOTSUP = (2),
    E_NOTFOUND = (3)
};


/* Constants */
#define REF_CTR_MARKER              (0xa5a5a5a5)
#define REF_CTR_MAX_MANAGED_LENG    (4 * 1024 * 1024)
#define SUCCESS (0)

/* Macros */
#define ReturnOnSuccess(ret)           if (ret == SUCCESS) return(ret);
#define ReturnOnError(ret)             if (ret != SUCCESS) return(ret);
#define RetOnErrorWithLog(ret, ...) \
if (ret != SUCCESS) \
{ \
    E_Printf(__VA_ARGS__); \
    return(ret); \
}

#define TO_DWORD(b0, b1, b2, b3) (((b3) << 24) | ((b2) << 16) | ((b1) << 8) | (b0))
#define BUF_TO_DWORD(in_buf, ofst) TO_DWORD((in_buf[(ofst)]), \
                                          (in_buf[(ofst) + 1]), \
                                          (in_buf[(ofst) + 2]), \
                                          (in_buf[(ofst) + 3]))


typedef struct RefCtr_s {
    uint32_t marker;
    int ct;
    size_t blen;
} RefCtr_t;
#define RefCtd  RefCtr_t ref


typedef struct RefCtdObj_s {
    RefCtd;
} RefCtdObj_t;


/* Ideally, this should be a function. But, in C, a powerful macro preprocessing
 * mitigates the lack of reflections. Hence, a complex macro! */
#define AssertMarker(invar) \
    if ((invar)->ref.marker != REF_CTR_MARKER) \
    { \
        LOG_Printf(ERROR, "Missing marker for word_list_t *0x%llx! Aborting...", (uint64_t) (invar)); \
        cleanup_and_exit(EINVAL); \
    }

#define SharedPtr(intype, invar) \
    AssertMarker(invar); \
    LOG_Printf(DEBUG, "Ref count up: (0x%llx)", (unsigned long long) invar); \
    __attribute__((cleanup (ref_ct_dwn))) RefCtdObj_t *scoped##var = (RefCtdObj_t *) invar; \
    invar->ref.ct++;


void *vl_alloc(size_t inlen);
void ref_ct_dwn(RefCtdObj_t** bufp);
void cleanup_and_exit(int errcode);


static inline __attribute__((always_inline)) int bit_val_at(uint8_t* bm,
        int bitnum)
{
    uint8_t bitmask = (1 << (bitnum & 0x07));
    return (bm[bitnum >> 3] & bitmask);
}



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GENERICS_H__ */
