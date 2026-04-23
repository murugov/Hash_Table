#include "logger.hpp"


<<<<<<< HEAD
FILE *log_file = NULL;
const char* verdict_strings[] = {"INFO", "DEBUG", "WARN", "ERROR"};

int open_log_file(const char* path)     // make it possible to create multiple logfiles
{
    log_file = fopen(path, "w");
    if ((void*)log_file == NULL)
=======
FILE *LogFile = NULL;
const char* verdict_strings[] = {"INFO", "DEBUG", "WARN", "ERROR"};

int LogFileOpener(const char* path)     // make it possible to create multiple logfiles
{
    LogFile = fopen(path, "w");
    if ((void*)LogFile == NULL)
>>>>>>> ce9a91af20a404497839faef230d97ce2a710d85
    {
        perror("Failed to open log file");
        return 1;
    }
    return 0;
}

<<<<<<< HEAD
int close_log_file()
{
    if ((void*)log_file == NULL)
    {
        fclose(log_file);
=======
int LogFileCloser()
{
    if ((void*)LogFile == NULL)
    {
        fclose(LogFile);
>>>>>>> ce9a91af20a404497839faef230d97ce2a710d85
        return 0;
    }
    return 1;
}