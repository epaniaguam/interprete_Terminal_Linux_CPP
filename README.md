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

| Comandos  | Descripción                                             |
|-----------|---------------------------------------------------------|
| cp        | Copia archivos y directorios.                            |
| ls        | Lista los contenidos de un directorio.                   |
| rm        | Elimina archivos o directorios.                          |
| touch     | Crea un archivo vacío o actualiza su marca de tiempo.    |
| mkdir     | Crea un nuevo directorio.                                |
| less      | Permite visualizar un archivo de texto página por página. |
| nano      | Editor de texto en la terminal.                          |
| cat       | Muestra el contenido de un archivo.                      |
| pwd       | Muestra la ruta del directorio actual.                   |
| ip addr   | Muestra y configura información de red.                   |
| less/more | Visualiza un archivo de texto página por página.          |
| ps        | Muestra los procesos en ejecución.                       |
| mv        | Mueve o renombra archivos y directorios.                  |
| vim       | Editor de texto en la terminal.                          |


## Comando para salir del terminal
Para finalizar la ejecución de la terminal deberemos introducir el comando "salir".
```bash
  salir
```
> [!WARNING]
> El comando de "cd" no es funcional se debe utilizar rutas absolutas o relativas para ejecutar los comandos

> [!NOTE]
> En caso se quiera ejecutar un comando distinto al de la carpeta /bin/ se debe especificar al inicio del comando.


