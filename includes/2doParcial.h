#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sock-lib.h>

#define FILENAME "data.parcial"
#define UNIT "[V/m] (Volt por metro)"
#define UNIT_MAX_SIZE 32 /*Maximo valor de longitud de */

#define BUFFSIZE 1024

struct efd
{
	char * unidades;
	int x; // cantidad de puntos de ancho de la distribución en el plano
	int y; // cantidad de puntos de largo de la distribución en el plano
	int z; // cantidad de puntos de alto de la distribución en el espacio
	float * values; // Total de valores de la distribución ( x * y * z)
}; 

typedef struct efd EFD;

// Espacio para agregar los prototipos de las funciones que implementar´a en los tres modulos
//Server
void SigHandler(int sigid);

void ReadFileName(EFD * ms, int sockid, int FD_in, int FD_out, char ** lastbuffer);
void SendFileName(int FD, char ** buff);
void ReadAndSend(EFD * ms, int sockid, int FD_out);
void myExit(EFD * ms, char ** lastbuffer, int FD_out, int FD_in, int sockid);
void nofilecheck(EFD * ms, int sockid, char ** lastbuffer, int FD_out, int FD_in);

//Reader
void OpenRedrFIFOs(int * FD_in, int * FD_out);
void ReadFileNameRedr(int FD_in, int FD_out, char ** buffer, char ** lastbuffer);
void FileError(int FD_out, int FD_in, char ** buffer);
void SendValues(EFD * ms, int FD_in, int FD_out, int FD_file, char ** buffer);

//Cliente
void SendFileName(int FD, char ** buff);
void ReadFromServer(int FD, EFD * stru, char ** buff);
void PrintData(EFD * stru);