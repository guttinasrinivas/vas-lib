#ifndef __TGL_LOGGER_H__
#define __TGL_LOGGER_H__
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <memory>


#define CRITICAL    (0)
#define ERROR       (1)
#define WARNING     (2)
#define INFO        (3)
#define VERBOSE     (4)
#define DEBUG       (5)


static const std::string default_log_fname = "output.log";


class SimpleLogger
{
    public:
        SimpleLogger(const std::string& logfname=default_log_fname, int loglevel=SimpleLogger::Debug);

        SimpleLogger(const SimpleLogger&) = delete;
        SimpleLogger(SimpleLogger&&) = delete;
        void operator=(const SimpleLogger&) = delete;
        void operator=(SimpleLogger&&) = delete;

        virtual ~SimpleLogger(void);
        void log(int level, std::string &logstr);
        void log(int level, const char *logstr);

    public:
        static auto getLogger(const std::string& logfname=default_log_fname, int loglevel=SimpleLogger::Debug)
        {
            static auto logger_ = std::make_shared<SimpleLogger>(logfname, loglevel);

            return logger_;
        }

        auto getLevel(void) { return level; }
        void setLevel(int inlevel) { level = inlevel; }

    public:
        static const int SBUF_LENGTH = 4096;
        enum
        {
            Critical = 0,
            Error = 1,
            Warning = 2,
            Info = 3,
            Verbose = 4,
            Debug = 5
        };

    public:
        char sbuf[SimpleLogger::SBUF_LENGTH];

    protected:
        int level;
        const std::string fname_;
        FILE *log_fp_;

    protected:
        static std::shared_ptr<SimpleLogger> logger_;
};
using SimpleLogger_p = std::shared_ptr<SimpleLogger>;


#ifndef LOG_DISABLED
#define LOG_MultiBuf(lvl, ...) \
    snprintf(logger->sbuf, SimpleLogger::SBUF_LENGTH, ##__VA_ARGS__); \
logger->log(lvl, logger->sbuf)
#else /* LOG_DISABLED */
#define LOG_MultiBuf(lvl, ...) {}
#endif /* LOG_DISABLED */


#define LOG_Debug(...)      LOG_MultiBuf(SimpleLogger::Debug, ##__VA_ARGS__)
#define LOG_Verbose(...)    LOG_MultiBuf(SimpleLogger::Verbose, ##__VA_ARGS__)
#define LOG_Info(...)       LOG_MultiBuf(SimpleLogger::Info, ##__VA_ARGS__)
#define LOG_Warning(...)    LOG_MultiBuf(SimpleLogger::Warning, ##__VA_ARGS__)
#define LOG_Error(...)      LOG_MultiBuf(SimpleLogger::Error, ##__VA_ARGS__)
#define LOG_Critical(...)   LOG_MultiBuf(SimpleLogger::Critical, ##__VA_ARGS__)

#endif /* __TGL_LOGGER_H__ */


/* End of file */
