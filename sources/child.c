#include <2doParcial.h>

void ReadFileName(EFD * ms, int sockid, int FD_in, int FD_out, char ** lastbuffer)
{
	char * tempbuff = malloc(BUFFSIZE);
	while(strcmp(tempbuff, "FIN") != 0)
	{
		strcpy(*lastbuffer, tempbuff);
		read(sockid, tempbuff, BUFFSIZE);
	}
	//
	if(strcmp(*lastbuffer, "shutdown") == 0)
	{
		fprintf(stdout, "Comando recibido, cerrando..\n");
		//
		FD_in = open("./pipes/reader_IN_FIFO", O_WRONLY);
		FD_out = open("./pipes/reader_OUT_FIFO", O_RDONLY);
		//No hace falta pasar los file descriptors por referencia porque despues uso exit()
		write(FD_in, "shutdown", BUFFSIZE);
		write(FD_in, "FIN", BUFFSIZE);
		kill(getppid(), SIGUSR1);
		myExit(ms, lastbuffer, FD_out, FD_in, sockid);
	}
}

void SendFileName(int FD, char ** buff)
{
	fprintf(stdout, "--\\%d: Enviando a reader..\n", getpid());
	write(FD, *buff, BUFFSIZE);
	write(FD, "FIN", BUFFSIZE);
}

void ReadAndSend(EFD * ms, int sockid, int FD_out)
{
	int Readed = 0;
	int ReadedTotal = 0;
	int Enviados = 0;
	int xyz = 1; //Donde almaceno el producto de x*y*z
	int readloop = 0; //Lo uso para el loop al leer valor por valor hasta llegar a xyz
	char * tempbuff = malloc(BUFFSIZE);
	//
	write(sockid, ms->unidades, UNIT_MAX_SIZE);

	Readed = read(FD_out, tempbuff, sizeof(int));
	write(sockid, tempbuff, sizeof(int));
	ms->x = atoi(tempbuff);

	Readed = read(FD_out, tempbuff, sizeof(int));
	write(sockid, tempbuff, sizeof(int));
	ms->y = atoi(tempbuff);

	Readed = read(FD_out, tempbuff, sizeof(int));
	write(sockid, tempbuff, sizeof(int));
	ms->z = atoi(tempbuff);

	xyz = ms->x * ms->y * ms->z;
	ms->values = calloc(xyz, sizeof(float));

	while(readloop ++ < xyz)
	{	
		memset(tempbuff, '\0', BUFFSIZE);
		Readed = read(FD_out, tempbuff, sizeof(float));
		ReadedTotal += Readed;
		
		if(Readed == -1)
			fprintf(stderr, "--\\%d: Error en el mensaje del FIFO.\n", getpid());
		
		*(ms->values + readloop) = atof(tempbuff);
		Enviados += write(sockid, tempbuff, sizeof(float));
	}
	fprintf(stdout, "--\\%d: %d/%d bytes (leidos/enviados), %d/%d valores (leidos/enviados)\n",
		getpid(), ReadedTotal, Enviados, readloop - 1, xyz);
	//
	free(tempbuff);
	//
	fprintf(stdout, "--\\%d: Finalizando proceso\n", getpid());
}

void nofilecheck(EFD * ms, int sockid, char ** lastbuffer, int FD_out, int FD_in)
{
	if(strcmp(ms->unidades, "nofile") == 0)
	{
		write(sockid, "nofile", UNIT_MAX_SIZE);
		fprintf(stderr, "--\\%d: ** Error en la apertura del archivo.\n", getpid());
		fprintf(stdout, "--\\%d: Finalizando proceso\n", getpid());
		myExit(ms, lastbuffer, FD_out, FD_in, sockid);
	}
}

void myExit(EFD * ms, char ** lastbuffer, int FD_out, int FD_in, int sockid)
{
	free(*lastbuffer);
	free(ms->unidades);
	free(ms->values);
	close(FD_out);
	close(FD_in);
	close(sockid);
	exit(1);
}