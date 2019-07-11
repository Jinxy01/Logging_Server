#include "cs.h"

Mesg mesg;

int server(int ipcreadfd, int ipcwritefd, int readfd, int writefd){

	int n, fd;
	extern int errno;

	int pid=fork();

	/*Filho para message queue*/
	if(pid==0){

		fd = open("log.txt", O_WRONLY);
		mesg.mesg_type = 1L;

		while(1){

			lseek(fd, 0, SEEK_END);

			if ((n = mesg_recv(ipcreadfd, &mesg)) < 0)
				err_sys("1 - Server: filename read error!\n");
			
			mesg.mesg_data[n] = '\0';

			write(fd, mesg.mesg_data, mesg.mesg_len); 
			write(fd, "\n", 1);

			mesg.mesg_len = 0;
			mesg_send(ipcwritefd, &mesg);

		}
	}
	/*Pai para admin*/
	else{

		char            buff[MAXBUFF+4];
		char            errmesg[256];
		int             n, fd;
		extern int      errno;
		memset(buff,'\0',MAXBUFF);

		while(1){
			/* Ler buffer */ 
			memset(buff,'\0',MAXBUFF);
			if ((n = read(readfd, buff,MAXBUFF )) == -1)
			{
				err_sys("Server: filename read error!");
			}


			if(n==0)
				continue;

			/* Opçao pass */ 
			if(!strncmp(buff+1,"pass",4)){
				if (buff[0]=='1'){ 
					memset(buff,'\0',MAXBUFF);
					char str[]="Ja introduziu a pass!!\n";
					n = strlen(str);
					memcpy(buff,&n,4);
					strcat(buff+4, str);
					write(writefd,buff,MAXBUFF+4);
				}

				else if(!strncmp(buff+6,"123",3)){
					memset(buff,'\0',MAXBUFF);
					write(writefd,"1",MAXBUFF);		
					continue;
				}
				else{			
					memset(buff,'\0',MAXBUFF);
					write(writefd,"0",MAXBUFF);
					continue;
				}
					
			}
			/* Ver logo de todos os clientes */
			else if (!strncmp(buff+1,"ler ",4)){
				if ((fd = open(buff+5, 0)) < 0)
				{
					if (buff[0]=='0'){
						memset(buff,'\0',MAXBUFF);
						write(writefd,"Sem permissao!\n",15);
						
					}else{
					/*
					 * Error. Format an error message and send it back to the
					 * client.
					*/
					sprintf(errmesg, "File not found! Error: %s\n", sys_err_str());
					

					memset(buff,'\0',MAXBUFF+4);
					n = strlen(errmesg);
					memcpy(buff,&n,4);
					strcat(buff+4, errmesg);

					if (write(writefd, buff, n+4) != n+4)
					{
						err_sys("Server: errmesg write error!");

					}
					}
				} 
				else
				{


					/*
					 * Read the data from the file and write to the IPC
					 * descriptor.
					*/
					memset(buff,'\0',MAXBUFF+4);
					while ((n = read(fd, buff+4, MAXBUFF)) > 0)
					{

						memcpy(buff,&n,4);
						if (write(writefd, buff, MAXBUFF+4) < n)
						{
							err_sys("Server: data write error!\n");

						}
					}
					if (n < 0)
					{

						err_sys("Server: read error!\n");

					}
				}
			}
			
			else{
				if (buff[0]=='0'){
						memset(buff,'\0',MAXBUFF);
						write(writefd,"Comando invalido!\n",19);}
				else{
					memset(buff,'\0',MAXBUFF);
					char str[]="Comando invalidoo!\n";
					n = strlen(str);
					memcpy(buff,&n,4);
					strcat(buff+4, str);
					write(writefd,buff,MAXBUFF+4);
				}
			}
		}
	}
	return 0;
}




				
