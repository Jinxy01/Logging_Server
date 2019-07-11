#include "cs.h"
#include "fifo.h"

int main()
{
	int readfd, writefd;
	int readid, writeid;

	umask(0);

		if ((mknod(FIFO1, S_IFIFO | PERMS, 0) < 0) && (errno != EEXIST)){
			fprintf(stderr, "can't create fifo: %s", FIFO1);
			return (-1);

		}
		if ((mknod(FIFO2, S_IFIFO | PERMS, 0) < 0) && (errno != EEXIST)){
			unlink(FIFO1);
			fprintf(stderr, "can't create fifo: %s", FIFO2);
			return (-1);
		}
		
		if ((readfd = open(FIFO1, 0)) < 0){
			fprintf(stderr, "server: can't open read fifo: %s", FIFO1);
			return (-1);
		}
		if ((writefd = open(FIFO2, 1)) < 0){
			fprintf(stderr, "server: can't open write fifo: %s", FIFO2);
			return (-1);
		}

		/* Create the message queues, if required. */

		if ((readid = msgget(MKEY1, PERMS | IPC_CREAT)) < 0)
			err_sys("server: can't get message queue 1");

		if ((writeid = msgget(MKEY2, PERMS | IPC_CREAT)) < 0)
			err_sys("server: can't get message queue 2");

		server(readid, writeid,readfd,writefd);

	close(readfd);
	close(writefd);
	//close(readid);
	//close(writeid);
	
	return (0);

}
