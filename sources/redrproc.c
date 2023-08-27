#include <2doParcial.h>

void OpenRedrFIFOs(int * FD_in, int * FD_out)
{
	fprintf(stdout, "Abriendo FIFOs..\n");
	*FD_in = open("./pipes/reader_IN_FIFO", O_RDONLY);
	*FD_out = open("./pipes/reader_OUT_FIFO", O_WRONLY);
	if(*FD_out == -1 || *FD_in == -1)
	{
		perror("** Error al abrir FIFO");
		exit(1);
	}
	fprintf(stdout, "FIFOs abiertos.\n");
}

void ReadFileNameRedr(int FD_in, int FD_out, char ** buffer, char ** lastbuffer)
{
	fprintf(stdout, "Esperando mensaje de PIPE..\n");
	while(strcmp(*buffer, "FIN") != 0)
	{
		strcpy(*lastbuffer, *buffer);
		read(FD_in, *buffer, BUFFSIZE);
		//getchar();
	}
	fprintf(stdout, "Recibido: '%s'\n", *lastbuffer);

	/* Verifico que no sea cmd especial */
	if(strcmp(*lastbuffer, "shutdown") == 0)
	{
		fprintf(stdout, "Finalizando programa.\n");
		close(FD_in);
		close(FD_out);
		//close(FD_file);
		free(*buffer);
		free(*lastbuffer);
		exit(1);
	}
}

void FileError(int FD_out, int FD_in, char ** buffer)
{
	perror("** Error al abrir archivo");
	write(FD_out, "nofile", 32);

	close(FD_in);
	close(FD_out);
	memset(*buffer, '\0', BUFFSIZE);
	fprintf(stdout, "======================================================\n");
	sleep(1);
}

void SendValues(EFD * ms, int FD_in, int FD_out, int FD_file, char ** buffer)
{
	int Readed = 0;
	int Enviados = 0;
	int readloop = 0;
	int xyz = 1;

	/* Leo y mando "unidades" */
	Readed = read(FD_file, *buffer, UNIT_MAX_SIZE);
	strcpy(ms->unidades, *buffer);
	write(FD_out, ms->unidades, UNIT_MAX_SIZE);
	fprintf(stdout, "'Unidades' enviadas.\n");

	/* Leo y mando "x", "y", "z" por a parte */
	Readed = read(FD_file, *buffer, sizeof(int));
	sprintf(*buffer, "%d", **buffer);
	write(FD_out, *buffer, sizeof(int));
	ms->x = atoi(*buffer);
	fprintf(stdout, "(x = %d, ", ms->x);

	Readed = read(FD_file, *buffer, sizeof(int));
	sprintf(*buffer, "%d", **buffer);
	write(FD_out, *buffer, sizeof(int));
	ms->y = atoi(*buffer);
	fprintf(stdout, "y = %d, ", ms->y);
			
	Readed = read(FD_file, *buffer, sizeof(int));
	sprintf(*buffer, "%d", **buffer);
	write(FD_out, *buffer, sizeof(int));
	ms->z = atoi(*buffer);
	fprintf(stdout, "z = %d)\n", ms->z);

	fprintf(stdout, "'x', 'y', 'z' enviadas.\n");

	xyz = ms->x * ms->y * ms->z;
	ms->values = calloc(xyz, sizeof(float));
	fprintf(stdout, "Calculados %d valores a enviar.\n", xyz);

	/* Leo y mando todo lo demas */
	Readed = 0;
	while(readloop ++ < xyz)
	{
		memset(*buffer, '\0', BUFFSIZE);
	
		Readed += read(FD_file, *buffer, sizeof(float));
		sprintf(*buffer, "%f", (float)**buffer);
		*(ms->values + readloop) = atof(*buffer);
		Enviados += write(FD_out, *buffer, sizeof(float));
	}
	fprintf(stdout, "%d/%d bytes (leidos/enviados), %d/%d valores (leidos/enviados)\n", 
		Readed, Enviados, readloop - 1, xyz);

	free(ms->values);
}