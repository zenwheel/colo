#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

double gettime(void);

/*xterm path*/
#define XTERM "/usr/X11R6/bin/xterm"
#define DEFAULT_COLORC "/usr/local/lib/colorc"

main(argc,argv)
     int argc;
     char *argv[];
{
  FILE    *f;
  int i=0,c=0,t=0,g=0, new, mynum;
  char *argz[argc+6];
  char *out,*fg,*bg,*cur,homedir[60],colorc[60];
  double e_time, part;

  out = (char *) malloc(255);
  
  sprintf(homedir,"%s/.colorc",getenv("HOME"));

  if(f=fopen(homedir,"r")) {
    g=1;
    sprintf(colorc,homedir);
  }

  if(!g&&(f=fopen(DEFAULT_COLORC,"r"))) {
    sprintf(colorc,DEFAULT_COLORC);
    fprintf(stderr,"No .colorc, using global resource file, %s.\n",colorc);
    g=1;
  }

  /* g is set now if either the home/.colorc, or the global file exist */

  if(g) {
    while (fgets(out,999,f)) {
      i++;
      bg=out;
      if((*bg==' '||*bg=='\n'||*bg=='\0')&&(!strstr(out,","))) {
	fprintf(stderr,"Your resource file sucks, using a standard xterm.\n");
	if(!fork()) {
	  execv(XTERM,argv);
	} else {
	  exit(0);
	}
      }
    }
    fclose(f);

    if(i<2) {
      fprintf(stderr,"Your resource file is missing information, using a standard xterm.\n");
      if(!fork()) {
	execv(XTERM,argv);
      } else {
	exit(0);
      }
    }

    f=fopen(colorc, "r");
    e_time = gettime();
    new = (e_time - (part= (int)e_time)) * 100000;
    srand(new);

    t = rand() % (i-1); /*this too*/
    t++;
    /* this one is so we can have a title on the first line */
    t++;
    while (fgets(out,999,f)) {
      out[strlen(out)-1]='\0';
      c++;
	if(t==c) break;
    }
    fclose(f);
    
    fg=out;
    bg=(char *)strstr(out,",");
    *bg='\0';
    bg++;
    if(strstr(bg,",")) {
      cur=(char *)strstr(bg,",");
      *cur='\0';
      cur++;
    }
    else {
      cur=fg;
    }
    
    if(*bg=='\0') {
      fprintf(stderr,"You are missing a background color to match %s, using default xterm.\n",fg);
      if(!fork()) {
	execv(XTERM,argv);
      } else {
	exit(0);
      }
      
    }
    if(*fg=='\0') {
      fprintf(stderr,"You are missing a foreground color to match %s, using default xterm.\n",bg);
      if(!fork()) {
	execv(XTERM,argv);
      }
      else {
	exit(0);
      }
    }
    
    argz[0]=argv[0];
    argz[1]="-fg";
    argz[2]=fg;
    argz[3]="-bg";
    argz[4]=bg;
    argz[5]="-cr";
    argz[6]=cur;
    argz[7]="-ms";
    argz[8]=cur;

    for(c=1;c<=argc;c++) {
      argz[c+8]=argv[c];
    }

    if(!fork()) {
      execv(XTERM,argz);
    }
    else {
      exit(0);
    }
  } else {
    fprintf(stderr,"Using a standard xterm.\n");
    if(!fork()) {
      execv(XTERM,argv);
    } else {
      exit(0);
    }
  }
  fprintf(stderr,"Whoah.. I can't even open an xterm, check the xterm path in the source.\n");
}

double gettime(void) {                          /* Used for wall time */
  double curtime;
  
  struct timeval {
    long tv_sec;
    long tv_usec;
  } end_time;

  struct timezone {
    int tzm;
    int tzz;
  }  tz;
  
  gettimeofday (&end_time, &tz);
  curtime = end_time.tv_sec + (end_time.tv_usec ) /100000.;
  return curtime;
}
