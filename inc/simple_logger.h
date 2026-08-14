#ifndef __SIMPLE_LOGGER_H__
#define __SIMPLE_LOGGER_H__

#ifndef __cplusplus
#include <stdint.h>
#include <stdio.h>
#include <time.h>


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
#define LOG_Debug(...) LOG_Printf(DEBUG, __VA_ARGS__)
#define LOG_Info(...)  LOG_Printf(INFO, __VA_ARGS__)
#define LOG_Warning(...) LOG_Printf(WARNING, __VA_ARGS__)
#define LOG_Error(...) LOG_Printf(ERROR, __VA_ARGS__)
#define LOG_Critical(...) LOG_Printf(CRITICAL, __VA_ARGS__)
#define E_Printf(...)            LOG_Printf(ERROR, __VA_ARGS__)


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
#else  /* __cplusplus */
#pragma message("For C++, use SimpleLogger.h instead")
#endif /* __cplusplus */


#endif /* __SIMPLE_LOGGER_H__ */
