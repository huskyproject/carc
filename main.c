#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "log.h"
#include "config.h"

int doPInbound();
int doXArc(char fname[12]);
int browseArgs(int argc, char *argv[]);
void printUsage();

int cmdX = 0;
int cmdA = 0;
int cmdAA = 0;

int isError = 0;


int main(int argc, char *argv[])
{
    char *logFileName;
    
    if (openConfig()) {
       printf("Error while opening the config file. Bailing out!\n");
       isError =1;
    }
    
    logFileName = (char *) malloc(strlen(getLogFileDir())+8+1);
    strcpy(logFileName, getLogFileDir());
    strcat(logFileName, "carc.log");

#ifdef DEBUGLEVEL
    if (openLog(logFileName, DEBUG)) {
        printf("Error opening log file. Bailing out.\n");
    }
#else
    if (openLog(logFileName, NORMAL)) {
        printf("Error opening log file. Bailing out.\n");
    }
#endif
    
    free(logFileName);
    
    if (browseArgs(argc, argv)) {
        addLog(NORMAL, "Error while parsing the Commandline");
        isError = 1;
    } else {
        addLog(DEBUG, "command line done");
    }

    if (cmdX) {
        if (doPInbound() == 0) {
            addLog(NORMAL, "Error while processing the PInbound. Bailing Out");
            isError = 1;
        } else {
            addLog(DEBUG, "eXpanded without errors.");
        }
    }

    closeConfig();
    closeLog();

    if (isError) {
        printf("There were errors during execution.\n");
        printf("Check the logfile for detailed information.\n");
        return 1;
    } else {
        return 0;
    }
}


int doPInbound()
{
    DIR *pinbound;
    struct dirent *fname;

    char *curdir;
    char *logentry;

    int errXArc;
    int xarced = 0;

    logentry = malloc(255);
    curdir = malloc(255);
    getcwd(curdir, 255);

    addLog(NORMAL, "processing pinbound");
    xarced = 0;

    if ((pinbound = opendir(getProtInbound()))) {
        chdir(getProtInbound());

        while ((fname=readdir(pinbound))) {
            /* ugly, how is ist done better? */
            if ((strstr(fname->d_name,".mo") ||
                 strstr(fname->d_name,".tu") ||
		 strstr(fname->d_name,".we") ||
		 strstr(fname->d_name,".th") ||
		 strstr(fname->d_name,".fr") ||
		 strstr(fname->d_name,".sa") ||
                 strstr(fname->d_name,".su") ||

                 strstr(fname->d_name,".MO") ||
       	         strstr(fname->d_name,".TU") ||
		 strstr(fname->d_name,".WE") ||
		 strstr(fname->d_name,".TH") ||
		 strstr(fname->d_name,".FR") ||
		 strstr(fname->d_name,".SA") ||
                 strstr(fname->d_name,".SU"))) {

                errXArc = doXArc(fname->d_name);

                if (errXArc) {
                    sprintf(logentry, "processing %s - error %d returned", fname->d_name, errXArc);
                } else {
                    sprintf(logentry, "processing %s - done", fname->d_name);
                    xarced++;
                }
                addLog(DEBUG, logentry);
            }
        }
        closedir(pinbound);
        chdir(curdir);
    }
    return xarced;
}


int doXArc(char fname[12])
{
    char arc[256];
    char cmd[256];

    int cmdexit;

    FILE *fhandle;
    char header[5];

    strcpy(arc, fname);

    if ((fhandle = fopen(arc, "r"))) {
        fgets(header, 5, fhandle);

        /* what kinda arc */
        if (header[0] == 0x60) {
            sprintf(cmd, "unarj e %s > /dev/null", fname);
        } else if (strstr(header, " ")) {
            printf("is ARC\n");
        } else if (strstr(header, "PK")) {
            sprintf(cmd, "unzip -j -Loqq %s%s -d%s", getProtInbound(),
                    fname, getProtInbound());
        } else if (strstr(header, "lh")) {
            printf("is LHA\n");
        } else if (strstr(header, "ZOO")) {
            printf("is ZOO\n");
        } else {
            addLog(NORMAL, "unable to determine arc type\n");
            return 1;
        }

        cmdexit = system(cmd);
        
        if (cmdexit) {
            return cmdexit;
            /* keep file */
        } else {
            sprintf(cmd, "rm %s", arc);
            system(cmd);
        }
    } else {
        printf("open failed: %s", arc);
        return 1;
    }
    return 0;
}


int browseArgs(int argc, char* argv[])
{
    if (argc < 2) {
        printUsage();
        return 1;
    } else {
        if (strcmp(argv[1], "x") == 0) {
            addLog(DEBUG, "eXpand requested");
            if (argc ==2) {
                cmdX = 1;
            } else {
                addLog(NORMAL, "to many parameters for command x. Bailing out");
                return 1;
            }
        } else if (strcmp(argv[1], "a") == 0) {
            printf("feature currently not supported\n");
        } else if (strcmp(argv[1], "a") == 0) {
            printf("feature currently not supported\n");
        }
        return 0;
    }
}


void printUsage()
{
    printf("carc usage:\n");
    printf("carc { x | a | aa}\n\n");
    printf("Commands:\n");
    printf("x  - eXpand files in protected inbound\n");
    printf("a  - attach file to node\n");
    printf("     a <file> <aka> <flavour>\n");
    printf("aa - arc attach a file to node\n");
    printf("     aa <file> <aka> <flavour> <packer>\n");
}

