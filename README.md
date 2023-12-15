# Interprete Terminal Linux en C++

Este proyecto es un intérprete de comandos de terminal Linux implementado en C++.

## Instalación

Para instalar este proyecto, sigue estos pasos:

1. Clona el repositorio: `git clone https://github.com/epaniaguam/interprete_Terminal_Linux_CPP.git`
2. Navega al directorio del proyecto: `cd interprete_Terminal_Linux_CPP`
3. Compila el proyecto: `g++ -o Interprete_Terminal Interprete_Terminal.cpp`

## Uso

Para usar este intérprete de terminal, ejecuta el programa compilado:

```bash
./Interprete_Terminal
```
## Comandos Testeados

| Comando          | Parámetros             | Descripción y Ejemplo                                      |
|------------------|------------------------|-------------------------------------------------------------|
| `cp`             | `<origen> <destino>`   | Copia archivos o directorios. Ejemplo: `cp archivo.txt carpeta/` |
| `ls`             | `-l`, `-a`, `-h`       | Lista archivos y directorios. Ejemplo: `ls -l`               |
| `rm`             | `-r`, `-f`             | Elimina archivos o directorios. Ejemplo: `rm -r carpeta/`     |
| `touch`          | `<archivo>`            | Crea un nuevo archivo vacío. Ejemplo: `touch nuevo.txt`      |
| `mkdir`          | `<nombre_directorio>`   | Crea un nuevo directorio. Ejemplo: `mkdir nueva_carpeta`    |
| `less`           | `<archivo>`            | Visualiza el contenido de un archivo de forma paginada. Ejemplo: `less archivo.txt` |
| `nano`           | `<archivo>`            | Editor de texto en la terminal. Ejemplo: `nano archivo.txt`   |
| `cat`            | `<archivo>`            | Muestra el contenido de un archivo. Ejemplo: `cat archivo.txt` |
| `pwd`            |                        | Muestra la ruta del directorio actual. Ejemplo: `pwd`        |
| `ip addr`        |                        | Muestra la configuración de red. Ejemplo: `ip addr show`     |
| `ps`             | `-aux`                 | Muestra los procesos en ejecución. Ejemplo: `ps aux`         |
| `mv`             | `<origen> <destino>`   | Mueve archivos o directorios. Ejemplo: `mv archivo.txt carpeta/` |
| `vim`            | `<archivo>`            | Editor de texto avanzado en la terminal. Ejemplo: `vim archivo.txt` |

Los comandos que siguen deben descargarse e instalarse en la linux antes de poder ejecutarse, para descargarlos se debe ejecutar "sudo apt install [Comandos Descargable]" fuera del programa

| Comandos Descargable | Descripción                                             |
|-----------|---------------------------------------------------------|
| neofetch | Muestra informacion util del sistema y sus componentes fisicos |
| tree | Muestra un árbol de los directorios |
| git | Muestra un árbol de los directorios |

## Comando para salir del terminal
Para finalizar la ejecución de la terminal deberemos introducir el comando "salir".
```bash
  salir
```
> [!WARNING]
> El comando de "cd" no es funcional se debe utilizar rutas absolutas o relativas para ejecutar los comandos

> [!NOTE]
> En caso se quiera ejecutar un comando distinto al de la carpeta /bin/ se debe especificar al inicio del comando.


