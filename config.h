/*
 file: config.h
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <fidoconf/fidoconf.h>

s_fidoconfig *config;

int openConfig();
char* getProtInbound();
char* getLocalInbound();
char* getOutbound();
char* getLogFileDir();
void closeConfig();

#endif
