#include <2doParcial.h>

void SendFileName(int FD, char ** buff)
{
	fprintf(stdout, "Conectado.\nIngrese el nombre del archivo:\n");
	scanf("%s", *buff);
	write(FD, *buff, BUFFSIZE);
	write(FD, "FIN", BUFFSIZE);
	/* salida */
	if(strcmp(*buff, "shutdown") == 0)
	{
		free(*buff);
		close(FD);
		exit(1);
	}
}

void ReadFromServer(int FD, EFD * stru, char ** buff)
{
	int xyz = 1; //Donde guardo el producto de x*y*z
	int readloop = 0; //Lo uso para el loop al leer valor por valor hasta llegar a xyz

	read(FD, *buff, UNIT_MAX_SIZE);

	if(strcmp(*buff, "nofile") == 0)
	{
		fprintf(stdout, "** Error: El archivo ingresado no existe.\n");
		free(*buff);
		close(FD);
		exit(1);
	}

	strcpy(stru->unidades, *buff);

	read(FD, *buff, sizeof(int));
	stru->x = atoi(*buff); 
	read(FD, *buff, sizeof(int));
	stru->y = atoi(*buff); 
	read(FD, *buff, sizeof(int));
	stru->z = atoi(*buff); 

	xyz = stru->x * stru->y * stru->z;

	stru->values = calloc(xyz, sizeof(float));

	while(readloop ++ < xyz)
	{	
		memset(*buff, '\0', BUFFSIZE);
		read(FD, *buff, sizeof(float));
		*(stru->values + readloop) = atof(*buff);
	}
}

void PrintData(EFD * stru)
{
	int xyz = stru->x * stru->y * stru->z;
	int count = 1;
	float num;

	fprintf(stdout, "Unidades:\n%s\n", stru->unidades);
	fprintf(stdout, "x: %d\n", stru->x);
	fprintf(stdout, "y: %d\n", stru->y);
	fprintf(stdout, "z: %d\n", stru->z);

	for(int i = 0; i < xyz; i ++)
	{
		printf("\t");
		num = *(stru->values + i);
		if(num < 0) fprintf(stdout, "%09.5f", num);
		else fprintf(stdout, "%08.5f", num);
		//
		if(count ++ == 8)
		{
			count = 1;
			printf("\n");
		}
	}
}