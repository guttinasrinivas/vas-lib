#include <memory>
#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include "SimpleLogger.h"


SimpleLogger::SimpleLogger(const std::string& logfname, int loglevel): level(loglevel),
                                                                 fname_(logfname)
{
    std::cout << "Logging to " << fname_ << ".\n";
    log_fp_ = fopen(fname_.c_str(), "a+");
}


SimpleLogger::~SimpleLogger(void)
{
}


void SimpleLogger::log(int lvl, std::string &logstr)
{
    log(lvl, logstr.c_str());
}


void SimpleLogger::log(int lvl, const char *logstr)
{
    char buf[64];
    if (lvl <= level)
    {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        fprintf(log_fp_, "%s: %s\n", buf, logstr);
    }
}

/* End of file */
