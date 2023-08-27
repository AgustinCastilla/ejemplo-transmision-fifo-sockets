#include <2doParcial.h>

int main(int argc, char * argv[])
{
	/* Verificamos ingreso de IP */
	if(argc < 2)
	{
		fprintf(stderr, "Uso: %s hostname [port]\n", argv[0]);
		exit(1);
	}
	/* Inicializamos variables */
	int FD_Server; //File descriptor del server
	char * buffer = malloc(BUFFSIZE);

	EFD myfiles;
	myfiles.unidades = malloc(UNIT_MAX_SIZE);

	/* Conectamos */
	fprintf(stdout, "Conectado a %s...\n", argv[argc-1]);
	FD_Server = conectar(argc, argv);

	/* Enviamos archivo */
	SendFileName(FD_Server, &buffer);

	/* Leo y imprimo */
	ReadFromServer(FD_Server, &myfiles, &buffer);
	PrintData(&myfiles);

	/* Devolvemos recursos */
	free(buffer);
	free(myfiles.unidades);
	free(myfiles.values);
	close(FD_Server);

	return 1;
}