Cliente=cli#Ejecutable del cliente
Server=srv#Ejecutable del server
Reader=redr#Ejecutable del reader
execs=cli srv redr#Lista de ejecutables
CC=gcc
CFLAGS=-c -I $(INCS_PATH) -Wall#Opciones para el compilador 
LDFLAGS=-lsock -no-pie -L $(LIBS_PATH) -Wall#Opciones para el linker
COBJS=cli.o cliproc.o#Lista de objetos que construye el cliente
SOBJS=srv.o child.o sgnHandler.o#Lista de objetos que construye el server
ROBJS=redr.o redrproc.o#Lista de objetos que construye el reader

SRCS_PATH=./sources
LIBS_PATH=./libs
INCS_PATH=./includes

DATE=$(shell date)

all: $(Cliente) $(Server) $(Reader)
$(Cliente): $(COBJS)
	$(CC) -o $@ $(COBJS) $(LDFLAGS)
$(Server): $(SOBJS)
	$(CC) -o $@ $(SOBJS) $(LDFLAGS)
$(Reader): $(ROBJS)
	$(CC) -o $@ $(ROBJS) $(LDFLAGS)

%.o: $(SRCS_PATH)/%.c Makefile
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f $(execs)

new: clean all

git:
	git add sources includes Makefile 
	git commit -m "Nuevo release $(DATE)"
	git push
