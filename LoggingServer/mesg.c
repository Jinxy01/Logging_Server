#include "cs.h"

void  mesg_send(int id, Mesg * mesgptr){
	if (msgsnd(id, mesgptr, mesgptr->mesg_len, 0) != 0)
		err_sys("msgsnd error");
}

int  mesg_recv(int id, Mesg * mesgptr){
	int n;
	n = msgrcv(id, mesgptr, sizeof(Mesg), 1l, 0);
	if ((mesgptr->mesg_len = n) < 0)
		err_dump("msgrcv error");
	
	return (n);
}
