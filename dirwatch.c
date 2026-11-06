#define _POSIX_C_SOURCE 200809L
#include <sys/inotify.h>
#include <poll.h>
#include <signal.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
static volatile sig_atomic_t running=1;
static void stop(int signal_number){(void)signal_number;running=0;}
static void quoted(const char *text){
 putchar('"');for(const unsigned char*p=(const unsigned char*)text;*p;p++){
  if(*p=='"'||*p=='\\'){putchar('\\');putchar(*p);}
  else if(*p<32||*p>=127)printf("\\u%04x",(unsigned)*p);
  else putchar(*p);
 }putchar('"');
}
static const char*kind(uint32_t mask){
 if(mask&IN_Q_OVERFLOW)return "overflow";
 if(mask&IN_IGNORED)return "watch-removed";
 if(mask&IN_MOVED_FROM)return "moved-from";
 if(mask&IN_MOVED_TO)return "moved-to";
 if(mask&IN_CREATE)return "created";
 if(mask&IN_DELETE)return "deleted";
 if(mask&IN_CLOSE_WRITE)return "write-closed";
 if(mask&IN_ATTRIB)return "attributes";
 if(mask&IN_DELETE_SELF)return "directory-deleted";
 if(mask&IN_MOVE_SELF)return "directory-moved";
 return "other";
}
int main(int argc,char **argv){
 if(argc!=2||strcmp(argv[1],"--help")==0){fprintf(stderr,"Usage: dirwatch DIRECTORY\nWrites JSON Lines; watches immediate children only. Ctrl+C to stop.\n");return argc==2?0:2;}
 int fd=inotify_init1(IN_NONBLOCK|IN_CLOEXEC);if(fd<0){perror("inotify_init1");return 1;}
 int wd=inotify_add_watch(fd,argv[1],IN_ONLYDIR|IN_CREATE|IN_DELETE|IN_MOVED_FROM|IN_MOVED_TO|IN_CLOSE_WRITE|IN_ATTRIB|IN_DELETE_SELF|IN_MOVE_SELF);
 if(wd<0){perror("inotify_add_watch");close(fd);return 1;}
 struct sigaction action;memset(&action,0,sizeof action);action.sa_handler=stop;sigemptyset(&action.sa_mask);sigaction(SIGINT,&action,NULL);sigaction(SIGTERM,&action,NULL);
 setvbuf(stdout,NULL,_IOLBF,0);fprintf(stderr,"Watching %s\n",argv[1]);
 union {char bytes[65536];struct inotify_event align;} buffer;
 while(running){
  struct pollfd pfd={fd,POLLIN,0};int ready=poll(&pfd,1,500);if(ready<0){if(errno==EINTR)continue;perror("poll");break;}if(!ready)continue;
  ssize_t n=read(fd,buffer.bytes,sizeof buffer.bytes);if(n<0){if(errno==EAGAIN||errno==EINTR)continue;perror("read");break;}
  for(size_t offset=0;offset<(size_t)n;){
   const struct inotify_event *event=(const struct inotify_event*)(buffer.bytes+offset);
   printf("{\"time\":%lld,\"event\":",(long long)time(NULL));quoted(kind(event->mask));printf(",\"name\":");quoted(event->len?event->name:"");printf(",\"cookie\":%u,\"directory\":%s}\n",event->cookie,(event->mask&IN_ISDIR)?"true":"false");
   if(event->mask&IN_Q_OVERFLOW)fprintf(stderr,"Event queue overflow: changes were lost; rescan your directory.\n");
   if(event->mask&IN_IGNORED)running=0;
   offset+=sizeof(struct inotify_event)+event->len;
  }
 }
 inotify_rm_watch(fd,wd);close(fd);return 0;
}
