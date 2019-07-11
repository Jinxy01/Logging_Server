#include "cs.h"

Mesg mesg;

int client(int ipcreadfd, int ipcwritefd){
    int n;
    char pid[64];
    time_t t;
    struct tm *tm;
    char date[64], str[MAXMESGDATA];

    printf("Bem-vindo! Introduza 'sair' para terminar sessão\n\n");

    while(1){

        memset(mesg.mesg_data, 0, MAXMESGDATA);
        t = time(NULL);
        tm = localtime(&t);
	sprintf(pid, "%d", getpid());
	strftime(date, sizeof(date), "%c", tm);
	mesg.mesg_type = 1L;

        printf("Utilizador %s >> ", pid);

        if (fgets(str, MAXMESGDATA, stdin) == NULL)
            err_sys("filename read error");

        if(strncmp(str, "sair", 4) == 0){
            exit(0);
        }

        n = strlen(str);
        if (str[n - 1] == '\n')
            str[n - 1] = '\0';

	strcat(mesg.mesg_data, "User: ");
        strcat(mesg.mesg_data, pid);
        strcat(mesg.mesg_data, " || Message:  ");
        strcat(mesg.mesg_data, str);

        n = strlen(date);
        if (date[n - 1] == '\n')
            date[n - 1] = '\0';

        strcat(mesg.mesg_data, " || Date: ");
        strcat(mesg.mesg_data, date);

        mesg.mesg_len = strlen(mesg.mesg_data);
        mesg_send(ipcwritefd, &mesg);   
    }
    return 0;
}
