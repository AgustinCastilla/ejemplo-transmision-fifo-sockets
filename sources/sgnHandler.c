#include <2doParcial.h>

void SigHandler(int sigid)
{
	if(sigid == SIGCHLD)
	{
		int n;
		waitpid(-1, &n, 0);
		//wait(NULL);
		printf("Proceso hijo terminado.\n");
	}
	if(sigid == SIGUSR1)
	{
		int n;
		waitpid(-1, &n, 0);
		//wait(NULL);
		printf("Terminando conexion..\n");
		exit(1);
	}
}