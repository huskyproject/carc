#include "config.h"

int openConfig()
{
    config = readConfig();
    return !config;
}

char* getProtInbound() {
    return config->protInbound;
}


char* getLocalInbound() {
    return config->localInbound;
}


char* getOutbound() {
    return config->outbound;
}


char* getLogFileDir() {
    return config->logFileDir;
}


void closeConfig()
{
    disposeConfig(config);
}
