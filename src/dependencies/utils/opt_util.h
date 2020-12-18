#ifndef _OPT_UTIL_H
#define _OPT_UTIL_H

#include <iostream>
#ifdef _MSC_VER
    extern int getopt(int argc, char * const argv[], const char *optstring) ;
    extern char *optarg ;
    extern int optind;
#else // _MSC_VER
    #include <unistd.h>
#endif //_MSC_VER

namespace ModGen {

#define DEFAULT_CONF_FILEPATH "./Conf/conf_test.xml" /*!< Fichier de conf par défaut          */
#define DEFAULT_LOGS_FILEPATH "./Logs/default_logs"  /*!< Fichier de logs par défaut          */
#define DEFAULT_LOGS_TRACEVAL 0                      /*!< Valeur de trace des logs par défaut */

inline void display_help()
{
    std::cout << "\nUsage modelGenerator <-c -e> [-l -t -T -S]"                          << std::endl;
    std::cout << "---Available options---"                                               << std::endl;
    std::cout << "====Required===="                                                      << std::endl;
    std::cout << "\t-c 'conf_filePath' : The configuration file path."                   << std::endl;
    std::cout << "\t-e 'sending_method': The method used to send the messages."          << std::endl;
    std::cout << "\t( '0':SIMPLE | '1':Smart Multiplex | '2':Full Multiplex | '3': AUTO)\n"
                                                                                         << std::endl;
    std::cout << "====Optional===="                                                      << std::endl;
    std::cout << "\t-l 'log_filePath': The file path used for logs. If not specified, no log will be created."
                                                                                         << std::endl;
    std::cout << "\t-t 'logs_traceLevel': The trace level to use for logs. (Default =  "
              << DEFAULT_LOGS_TRACEVAL << ")"                                            << std::endl;
    std::cout << "\t ( '-1':No logs | 0':Errors only | '1':Errors && infos | '2':Errors && infos && debug)" 
                                                                                         << std::endl;                             
    std::cout << "\t-T 'timeout': The timeout value in us for the AUTO mode. (Default = 1e6 = 1s)"
                                                                                         << std::endl; 
    std::cout << "\t-S 'max size': The maximum frame size in bytes for the AUTO mode. (Default = 100 bytes)"
                                                                                         << std::endl;
    std::cout << "====Examples===="                                                      << std::endl;                                                                                                                                                                                                                                                                   
    std::cout << "\t./modelSender -c /home/user/conf.xml -e 1 -l ./logs -t 1"            << std::endl;
    std::cout << "\t./modelSender -e 3 -T 1000 -S 2500\n"                                << std::endl;
}

} // namespace ModGen

#ifdef _MSC_VER
// public domain implementation of getopt

/*
Date: Tue, 25 Dec 84 19:20:50 EST
From: Keith Bostic <harvard!seismo!keith>
To: genrad!sources
Subject: public domain getopt(3)

There have recently been several requests for a public
domain version of getopt(3), recently.  Thought this
might be worth reposting.

                Keith Bostic
                        ARPA: keith@seismo
                        UUCP: seismo!keith

======================================================================
In April of this year, Henry Spencer (utzoo!henry) released a public
domain version of getopt (USG, getopt(3)).  Well, I've been trying to
port some USG dependent software and it didn't seem to work.  The problem
ended up being that the USG version of getopt has some external variables
that aren't mentioned in the documentation.  Anyway, to fix these problems,
I rewrote the public version of getopt.  It has the following advantages:

        -- it includes those "unknown" variables
        -- it's smaller/faster 'cause it doesn't use the formatted
                output conversion routines in section 3 of the UNIX manual.
        -- the error messages are the same as S5's.
        -- it has the same side-effects that S5's has.
        -- the posted bug on how the error messages are flushed has been
                implemented.  (posting by Tony Hansen; pegasus!hansen)

I won't post the man pages since Henry already did; a special note,
it's not documented in the S5 manual that the options ':' and '?' are
illegal.  It should be obvious, but I thought I'd mention it...
This software was derived from binaries of S5 and the S5 man page, and is
(I think?) totally (I'm pretty sure?) compatible with S5 and backward
compatible to Henry's version.

                Keith Bostic
                        ARPA: keith@seismo
                        UUCP: seismo!keith

*UNIX is a trademark of Bell Laboratories

.. cut along the dotted line .........................................
*/

int ansi_c_needs_something_here_too;

#define NEED_GETOPT
#ifdef NEED_GETOPT
#include <stdio.h>
#include <string.h>

/*
 * get option letter from argument vector
 */
int	optind = 1,		/* index into parent argv vector */
        optopt;		/* character checked for validity */
char	*optarg;	/* argument associated with option */

#define BADCH	(int)'?'
static char  EMSG[]	="";
#define tell(s)	fputs(*nargv,stderr);fputs(s,stderr); \
                fputc(optopt,stderr);fputc('\n',stderr);return(BADCH);

int getopt(int nargc,char * const nargv[], const char *ostr)
{
        static char	*place = EMSG;	/* option letter processing */
        static const char	*lastostr = (char *) 0;
        register const char	*oli = NULL ;		/* option letter list index */

        /* LANCE PATCH: dynamic reinitialization */
        if (ostr != lastostr) {
                lastostr = ostr;
                place = EMSG;
        }
        if(!*place) {			/* update scanning pointer */
                if((optind >= nargc) || (*(place = nargv[optind]) != '-')
                                || ! *++place) {
                        place = EMSG;
                        return(EOF);
                }
                if (*place == '-') {	/* found "--" */
                        ++optind;
                        return(EOF);
                }
        }				/* option letter okay? */
        if ((optopt = (int)*place++) == (int)':' || !(oli = strchr(ostr,optopt))) {
                if(!*place) ++optind;
                tell(": illegal option -- ");
        }
        if (*++oli != ':') {		/* don't need argument */
                optarg = NULL;
                if (!*place) ++optind;
        }
        else {				/* need an argument */
                if (*place) optarg = place;	/* no white space */
                else if (nargc <= ++optind) {	/* no arg */
                        place = EMSG;
                        tell(": option requires an argument -- ");
                }
                else optarg = nargv[optind];	/* white space */
                place = EMSG;
                ++optind;
        }
        return(optopt);			/* dump back option letter */
}
#endif
#endif // _MSC_VER

#endif // _OPT_UTIL_H