Prueba# ejemplo-envio-audio-sockets

Programa solicitado en la 2da evaluación de la cursada de Informática 1:
Se necesita implementar un sistema compuesto por tres archivos ejecutables diferentes que interactuarán entre si por medio de diferentes mecanismos de intercomunicación de procesos vistos durante el curso.

![alt text](https://github.com/AgustinCastilla/ejemplo-transmision-fifo-sockets/blob/main/esquema.jpg?raw=true)

En el archivo '2do.ParcialInfoI - 20181203.pdf' se encuentra el parcial completo donde se puede ver de forma mas detallada.

# Preparado

## Instalacion

En el parcial se pedia tener 2 fifo en el directorio '/pipes' dentro de '/home', pero en esta versión se utilian dentro de '/pipes' en el directorio donde se ejecute el servidor.
En caso de necesitar generar los pipes puede utiliar el siguiente comando:
```
mkfifo reader_IN_FIFO
mkfifo reader_OUT_FIFO
```

## Compilado:

Utilice el siguiente comando para compilar todos los archivos:
```
make all
```

# Uso

### Server:
Es el encargado de recibir las conexiones, leer a los clientes y enviar la información a través del pipe.
Solo ejecute el programa.

### Reader:
Es el que leera los archivos que le envíe el servidor a través del pipe. 
Solo ejecute el programa.

###
Es el que enviará el archivo.
Ejecute el programa y luego escriba en la consola el archivo que desea enviar.

# Libreria

El proyecto utiliza la libreria proporcionada por la catedra de Informática 1 de la UTN FRBA (Alejandro Furfaro).
