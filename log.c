#include <time.h>
#include <string.h>
#include "log.h"


void dt2str(char *s)
{
	time_t timer;
	struct tm *tblock;

	timer = time(NULL);
	tblock = localtime(&timer);
	strftime(s, 19, "%b %d %y %H:%M:%S", tblock);
}



int openLog(char *logname, int loglevel)
{
    llevel = loglevel;
    if ((lfile = fopen(logname, "a"))) {
        if (loglevel == DEBUG) {
            addLog(NORMAL, "carc startet in debug level");
        } else {
            addLog(NORMAL, "carc startet");
        }
        return 0;
    } else {
        return (int) lfile;
    }
}


int addLog(int loglevel, char *loginfo)
{
    char foo[80];
    char dtstr[20];

    if (llevel < loglevel) {
        return 0;
    } else {
        (loglevel == DEBUG)?strcpy(foo, "#"):strcpy(foo, "-");
        dt2str(dtstr);
        sprintf(foo, "%s%s %s\n", foo, dtstr, loginfo);
        return (fprintf(lfile, foo));
    }
}


int closeLog(FILE *logfile)
{
    addLog(NORMAL, "carc finished");
    return (fclose(lfile));
}
