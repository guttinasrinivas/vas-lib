#ifndef __GENERICS_H__
#define __GENERICS_H__
#include <stdint.h>
#include <stdio.h>
#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Constants */
#define SUCCESS (0)

/* LOGGING STUFF */
/* LOG LEVELS */
#define CRITICAL    (1)
#define ERROR       (2)
#define WARNING     (3)
#define INFO        (4)
#define DEBUG       (5)


#define LOG_Printf(lvl, ...)    if (lvl <= LOG_CurrLevel) print_time(); \
    printf("- %s %s - ", __FILE_NAME__, __func__); \
    printf(__VA_ARGS__); printf("\n")
#define E_Printf(...)            LOG_Printf(ERROR, __VA_ARGS__)


/* Macros */
#define ReturnOnError(ret)             if (ret != SUCCESS) return(ret);
#define RetOnErrorWithLog(ret, ...) \
if (ret != SUCCESS) \
{ \
    E_Printf(__VA_ARGS__); \
    return(ret); \
}


typedef struct RefCtr_s
{
    uint32_t marker;
    int ct;
} RefCtr_t;
#define RefCtd  RefCtr_t ref


typedef struct RefCtdObj_s
{
    RefCtd;
} RefCtdObj_t;


/* Ideally, this should be a function. But, in C, a powerful macro preprocessing
 * mitigates the lack of reflections. Hence, a complex macro! */
#define AssertMarker(invar) \
    if ((invar)->ref.marker != 0xa5a5a5a5) \
    { \
        LOG_Printf(ERROR, "Missing marker for word_list_t *0x%lx! Aborting...", (uint64_t) (invar)); \
        cleanup_and_exit(EINVAL); \
    }

#define SharedPtr(intype, invar) \
    AssertMarker(invar); \
    LOG_Printf(DEBUG, "Ref count up: (0x%llx)", (unsigned long long) invar); \
    __attribute__((cleanup (ref_ct_dwn))) RefCtdObj_t *scoped##var = (RefCtdObj_t *) invar; \
    invar->ref.ct++;


static void print_time(void)
{
    time_t now = time(NULL);         // Get current time
    struct tm *t = localtime(&now);  // Convert to local time structure

    printf("%04d-%02d-%02d_%02d:%02d:%02d: ",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec);

    return;
}


extern int LOG_CurrLevel;


void cleanup_and_exit(int errcode);


static inline __attribute__((always_inline)) int bit_val_at(uint8_t *bm, int bitnum)
{
    uint8_t bitmask = (1 << (bitnum & 0x07));
    return (bm[bitnum >> 3] & bitmask);
}



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GENERICS_H__ */
