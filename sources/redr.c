#include <2doParcial.h>

int main(void)
{
	/* Inicializo variables */
	int FD_in, FD_out; //File descriptors de los FIFOs con el server
	int FD_file; //File descriptor del archivo a leer
	char * buffer = malloc(BUFFSIZE);
	char * lastbuffer = malloc(BUFFSIZE);

	/* loop */
	while(1)
	{
		/* Declaro la estructura */

		EFD myfiles;
		myfiles.unidades = malloc(UNIT_MAX_SIZE);

		/* Abro fifos */
		OpenRedrFIFOs(&FD_in, &FD_out);

		/* Me quedo esperando en el pipe */
		ReadFileNameRedr(FD_in, FD_out, &buffer, &lastbuffer);

		/* Abro archivo */
		fprintf(stdout, "Abriendo archivo..\n");
		FD_file = open(lastbuffer, O_RDONLY);
		
		if(FD_file == -1) FileError(FD_out, FD_in, &buffer);
		else
		{
			SendValues(&myfiles, FD_in, FD_out, FD_file, &buffer);
			/* Cierro para volver a abrir la proxima vez */
			free(myfiles.unidades);
			close(FD_in);
			close(FD_out);
			close(FD_file);
			memset(buffer, '\0', BUFFSIZE);
			fprintf(stdout, "======================================================\n");
			sleep(1);
			//El sleep no hace falta, funciona sin él,
			//pero lo agregué porqué si no redr se vuelve a conectar al pipe antes de que
			//el srv lo cierre.
			//Entonces el redr se queda en el read, personalmente prefiero que se quede en
			//el open :)
		}
	}
	printf("\n\n\nUSTED NUNCA DEBERIA LEER ESTO\n\n\n");
	return 1;
}