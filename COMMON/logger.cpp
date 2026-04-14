#include "logger.hpp"


FILE *LogFile = NULL;
const char* verdict_strings[] = {"INFO", "DEBUG", "WARN", "ERROR"};

int LogFileOpener(const char* path)     // make it possible to create multiple logfiles
{
    LogFile = fopen(path, "w");
    if ((void*)LogFile == NULL)
    {
        perror("Failed to open log file");
        return 1;
    }
    return 0;
}

int LogFileCloser()
{
    if ((void*)LogFile == NULL)
    {
        fclose(LogFile);
        return 0;
    }
    return 1;
}