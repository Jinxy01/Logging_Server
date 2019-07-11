#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <pthread.h>

extern int errno;

#define err_sys(STR) fprintf(stderr, STR)
#define err_sys2(STR, XXX) fprintf(stderr, STR, XXX)
#define err_dump(XXX) fprintf(stderr, XXX)
#define MAXBUFF 1024

#define MKEY1 0x11
#define MKEY2 0x12
#define PERMS 0666

#define MAXMESGDATA 1024

typedef struct{
    long int mesg_type;
    char mesg_data[MAXMESGDATA];
    int mesg_len;
} Mesg;

typedef struct{
	char msg[100];
	int tempo;
	int* flag;
}aviso_t;

int server(int, int,int,int);
int client(int, int);
int admin(int,int);

void mesg_send(int id, Mesg *mesgptr);
int mesg_recv(int id, Mesg *mesgptr);

char *sys_err_str(void);

