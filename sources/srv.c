
/*
 * El programa está terminado.
 * Ya que tuvimos tiempito para terminarlo me tomé el trabajo de agregarle
 * algunas features, como checkear que el archivo exista, y agregar un
 * comando de cliente 'shutdown', en donde cierra el servidor, y este
 * termina con el 'redr'
*/

#include <2doParcial.h>

int main(void)
{
	/* Inicializo variables */
	int FD_input; //File descriptor por donde escucho conexiones
	int FD_in, FD_out; //File descriptors de los FIFOs con el reader
	int PR_Fork; //PID del fork
	int SK_duplicado; //Duplicado del socket
	char * lastbuffer = malloc(BUFFSIZE);
	//Lo uso para cuando el cliente manda el "FIN", para no perder el nombre del archivo.

	struct sockaddr_in my_addr;

	signal(SIGCHLD, &SigHandler); //Evitar hijos zombies
	signal(SIGUSR1, &SigHandler); //La señal que manda el child para cerrar el server

	/* Creo conexion */
	fprintf(stdout, "Creando conexion...\n");		
	if((FD_input = Open_conection(&my_addr)) == -1)
	{
		perror("Falló la creación de la conexión"); 
		exit(1);
	}
	fprintf(stdout, "Conexion establecida.\n");

	/* Loop */
	while(1)
	{
		/* Escucho */
		fprintf(stdout, "Aceptando pedidos...\n");
		SK_duplicado = Aceptar_pedidos(FD_input);

		/* Creo child para que se encargue de la conexion */
		if((PR_Fork = fork()) != 0)
		{
			fprintf(stdout, "Conexion recibida, creado subproceso ID #%d\n", PR_Fork);
			close(SK_duplicado);
		}
		else
		{
			/* Declaro estructura */
			EFD myfiles;
			myfiles.unidades = malloc(UNIT_MAX_SIZE);

			/* Escucho al cliente */
			ReadFileName(&myfiles, SK_duplicado, FD_in, FD_out, &lastbuffer);

			/* Abro fifos */
			FD_in = open("./pipes/reader_IN_FIFO", O_WRONLY);
			FD_out = open("./pipes/reader_OUT_FIFO", O_RDONLY);

			/* Escribo por el fifo el archivo */
			SendFileName(FD_in, &lastbuffer);

			/* Leo y retransmito lo recibido por el FIFO (Nombre del file) */
			read(FD_out, myfiles.unidades, UNIT_MAX_SIZE);

			/* Checkeo */
			nofilecheck(&myfiles, SK_duplicado, &lastbuffer, FD_out, FD_in);

			/* Leo y mando */
			ReadAndSend(&myfiles, SK_duplicado, FD_out);
				
			/* Devuelvo recursos */
			myExit(&myfiles, &lastbuffer, FD_out, FD_in, SK_duplicado);
		}
	}
	return 1;
}