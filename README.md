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

| Comandos  |
|-----------|
| cp        |
| ls        |
| rm        |
| touch     |
| mkdir     |
| less      |
| nano      |
| cat       |
| pwd       |
| ip addr   |
| less/more |
| ps        |
| mv        |
| vim       |

## Comando para salir del terminal
Para finalizar la ejecución de la terminal deberemos introducir el comando "salir".
```bash
  salir
```
> [!WARNING]
> El comando de "cd" no es funcional se debe utilizar rutas absolutas o relativas para ejecutar los comandos

> [!NOTE]
> En caso se quiera ejecutar un comando distinto al de la carpeta /bin/ se debe especificar al inicio del comando.


