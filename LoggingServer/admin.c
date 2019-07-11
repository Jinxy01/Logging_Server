#include "cs.h"

#define flagToChar(f) flag==0? '0':'1'

void aviso(char* msg, int tempo,int* flag);
void* avisoWrapper(void* args);
int startServer ();

int serverOn=0;

int admin(int readfd, int writefd)
{

	char   buff[MAXBUFF+4];
	int    n,flag=0,tempo;


	/*
	 * Read the filename from the standard input, write it to the IPC
	 * descriptor
	*/


while(1){
	memset(buff,'\0',MAXBUFF);
	if(!flag){
		memset(buff,'\0',MAXBUFF);
		fprintf(stderr, "Introduza palavra-chave >> ");
		fgets(buff+1, MAXBUFF-1, stdin);

		if(!strncmp(buff+1,"sair",4))
			exit(0);
		
		if(!strncmp(buff+1,"killServer",10))
			system("killall -KILL server");
		
		/*Request to and read from server*/
		buff[0]=flagToChar(flag);
		write(writefd,buff,MAXBUFF);
		read(readfd, buff, MAXBUFF);
		
		if(buff[0]=='1'){
			flag=1;
			tempo=10;
			printf("\nAcesso durante %d segundos...\n\n",tempo);
			pthread_t t;
			aviso_t* avisoPtr=malloc(sizeof(aviso_t));
			strcpy(avisoPtr->msg,"Acabou o tempo\n");
			avisoPtr->tempo=tempo;
			avisoPtr->flag=&flag;
			pthread_create(&t,NULL,avisoWrapper,(void*)avisoPtr);
			memset(buff,'\0',MAXBUFF+4);
		}
		else if(buff[0]=='0'){
			memset(buff,'\0',MAXBUFF);
			printf("Palavra-chave incorreta\n");	
		}
		else{
			write(1,buff,MAXBUFF);
		}
			
	}
	else{	
		fprintf(stderr, "Introduza um comando >> ");
		memset(buff,'\0',MAXBUFF);
		buff[0]='1';
		
		if (fgets(buff+1, MAXBUFF, stdin) == NULL) {
			err_sys("Client: filename read error!");
		}
		/*Opções de comando*/
		if(!strncmp(buff+1,"sair",4))
			exit(0);

		else if(!strncmp(buff+1,"killServer",10))
			system("killall -KILL server");

		
		else if((!strncmp(buff+1,"ler ",4))||(!strncmp(buff+1,"pesquisar ",10))){
			
			/*Write to server*/
			n = strlen(buff);

			if (buff[n - 1] == '\n') {
				buff[n - 1] = '\0';	/* ignore newline from fgets() */
			}
			if (write(writefd, buff, MAXBUFF) != MAXBUFF) {
				err_sys("Client: filename write error!");
			}

			/* Read the data from the IPC descriptor and write to standard output */
			memset(buff,'\0',MAXBUFF+4);
			while ((n = read(readfd, buff, MAXBUFF+4)) >= 0) {
					int n2;
					memcpy(&n2,buff,4);
					if (write(1, buff+4, n2) != n2)	// fd 1 = stdout
						err_sys("Client: data write error!");
					if (n2<MAXBUFF)
						break;
				}		

			if (n < 0) {
				err_sys("Client: data read error!");
			}
		}
		else {

			if (strcmp(buff,"1\n")){
			printf("Comando não reconhecido!\n");}
		}
	}
}
	

	

	return 0;

}

void aviso(char* msg, int tempo,int* flag){

	while(tempo>=0){
		sleep(1);
		tempo--;
	}
	
	printf("\n\nAviso: %s",msg);
	*flag=0;
}

void* avisoWrapper(void* args){
	
	aviso_t *avisoPtr=(aviso_t*)args;
	aviso(avisoPtr->msg,avisoPtr->tempo,avisoPtr->flag);
	free(avisoPtr);
	return NULL;
} 


