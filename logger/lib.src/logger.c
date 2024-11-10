#include "logger.h"

int 
getTimeStr(char* buffer)
{
    size_t len = (size_t)Maxsize+1;
   
    time_t rawtime;
    struct tm * timeinfo;
    struct timespec res;

    time ( &rawtime );
    timeinfo = localtime(&rawtime);
    int x;
    clock_gettime(CLOCK_REALTIME,&res);
    x = snprintf(buffer, len, "%d-%d-%d %d:%d:%d:%06ld", timeinfo->tm_mday, timeinfo->tm_mon, 1900+ timeinfo->tm_year, \
    timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,res.tv_nsec);

    return x;
}


void sysDebugLogger(debugData *debugDataInfo,int dbglevel,const char *file,int line,const char *format,...)
{
    va_list vars;
    va_start(vars,format);

    int a,b;
    char buffer[Maxsize +1];
    a = getTimeStr(buffer);

    a = a + snprintf(buffer+a,Maxsize - a," %s:%d , L%d: ",file, line , dbglevel);
    vsnprintf(buffer+a,Maxsize - a,format,vars);
    fprintf(debugDataInfo->logOutFile, "%s\n",buffer);
}

void sysInfoLogger(debugData *debugDataInfo,const char *file,int line,const char *format,...)
{
    va_list vars;
    va_start(vars,format);

    int a,b;
    char buffer[Maxsize +1];
    a = getTimeStr(buffer);

    a = a + snprintf(buffer+a,Maxsize - a," %s:%d , INFO: ",file, line );
    vsnprintf(buffer+a,Maxsize - a,format,vars);
    fprintf(debugDataInfo->logOutFile, "%s\n",buffer);
}

void sysErrLogger(debugData *debugDataInfo,const char *file,int line,const char *format,...)
{
    va_list vars;
    va_start(vars,format);

    int a,b;
    char buffer[Maxsize +1];
    a = getTimeStr(buffer);

    a = a + snprintf(buffer+a,Maxsize - a," %s:%d , ERR: ",file, line );
    vsnprintf(buffer+a,Maxsize - a,format,vars);
    fprintf(debugDataInfo->logOutFile, "%s\n",buffer);
}

debugData *debugDataInfo;

int main()
{
    debugDataInfo = (debugData *)malloc(sizeof(debugData));
    memset(debugDataInfo,0,sizeof(debugData));
    snprintf(debugDataInfo->fileName,MAXFILENAMELEN,"%s","./test.log");
    debugDataInfo->logOutFile = fopen(debugDataInfo->fileName,"a");
    debugDataInfo->debugLevel = 2;
    DebugMsg(1,"This is debug message:%d",1);
    DebugMsg(2,"This is debug message:%d",1);
    DebugMsg(3,"This is debug message:%d",1);
    InfoMsg("This is info message  %d",2);
    ErrMsg("This is error message  %d",3);

    return 0;
}