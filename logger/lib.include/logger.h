#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAXFILENAMELEN 200
#define Maxsize 10000

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


typedef struct _debugData
{
    char fileName[MAXFILENAMELEN];
    FILE * logOutFile;
    int debugLevel;

}debugData;


int getTimeStr(char* buffer);

void sysDebugLogger(debugData *debugDataInfo,int dbglevel,const char *file,int line,const char *format,...);

void sysInfoLogger(debugData *debugDataInfo,const char *file,int line,const char *format,...);

void sysErrLogger(debugData *debugDataInfo,const char *file,int line,const char *format,...);

#define validateAndPrintDebugMsg(debugDataInfo,debuglevel,debuglogger,...) \
    if(debugDataInfo != NULL && debuglevel <=debugDataInfo->debugLevel)  \
    {    \
        debuglogger(debugDataInfo,debuglevel,__VA_ARGS__);  \
    } 

#define validateAndPrintInfoMsg(debugDataInfo,infologger,...) \
    if(debugDataInfo != NULL)  \
    {    \
        infologger(debugDataInfo,__VA_ARGS__);  \
    } 


#define validateAndPrintErrMsg(debugDataInfo,errlogger,...) \
    if(debugDataInfo != NULL)  \
    {    \
        errlogger(debugDataInfo,__VA_ARGS__);  \
    } 


#define printDebugInformation(debugDataInfo,debuglevel, ...) validateAndPrintDebugMsg(debugDataInfo,debuglevel,sysDebugLogger,__FILENAME__,__LINE__, __VA_ARGS__)

#define printInformation(debugDataInfo, ...) validateAndPrintInfoMsg(debugDataInfo,sysInfoLogger,__FILENAME__,__LINE__, __VA_ARGS__)

#define printError(debugDataInfo, ...) validateAndPrintErrMsg(debugDataInfo,sysErrLogger,__FILENAME__,__LINE__, __VA_ARGS__)

#define DebugMsg(debuglevel,...) printDebugInformation(debugDataInfo,debuglevel,__VA_ARGS__)


#define InfoMsg(...) printInformation(debugDataInfo,__VA_ARGS__)

#define ErrMsg(...) printError(debugDataInfo,__VA_ARGS__)