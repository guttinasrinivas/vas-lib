#ifndef __GENERICS_H__
#define __GENERICS_H__

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

#define LOG_PRINTF(lvl, ...)    if (lvl <= INFO) printf(__VA_ARGS__)
#define EPRINTF(...)            LOG_PRINTF(ERROR, __VA_ARGS__)


/* Macros */
#define RET_ON_ERR(ret)             if (ret != SUCCESS) return(ret);
#define RET_MSG_ON_ERR(ret, ...) \
if (ret != SUCCESS) \
{ \
    EPRINTF(__VA_ARGS__); \
    return(ret); \
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GENERICS_H__ */
