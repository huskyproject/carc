#ifndef LOG_H
#define LOG_H

#include <stdio.h>

/* loglevel */
#define NORMAL 1
#define DEBUG 2

FILE *lfile;
int llevel;

int openLog(char *logname, int loglevel);
int addLog(int loglevel, char *loginfo);
int closeLog();

#endif LOG_H

