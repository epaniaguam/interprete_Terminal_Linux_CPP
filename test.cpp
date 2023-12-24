#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;

    // Crear la tubería
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Crear un nuevo proceso
    if ((pid = fork()) == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Código del proceso hijo
        // Redirigir la salida estándar al extremo de escritura de la tubería
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);  // Cerrar el extremo de lectura de la tubería

        // Ejecutar el primer comando (ls -ls)
        execv("/bin/ls", (char *const[]){"/bin/ls", "-ls", nullptr});  // Reemplazar con tu comando y argumentos
        perror("execv");  // Si execv falla
        return 1;
    } else {
        // Código del proceso padre
        // Redirigir la entrada estándar al extremo de lectura de la tubería
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);  // Cerrar el extremo de escritura de la tubería

        // Crear un nuevo proceso para el segundo comando (grep a > a.txt)
        pid_t pid2;
        if ((pid2 = fork()) == -1) {
            perror("fork");
            return 1;
        }

        if (pid2 == 0) {
            // Código del segundo proceso hijo
            // Redirigir la salida estándar al archivo a.txt
            int fd = creat("a.txt", 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);

            // Ejecutar el segundo comando (grep a)
            execv("/bin/grep", (char *const[]){"/bin/grep", "a", nullptr});  // Reemplazar con tu segundo comando y argumentos
            perror("execv");  // Si execv falla
            return 1;
        } else {
            // Código del proceso padre
            // Esperar a que ambos procesos hijos terminen
            wait(nullptr);
            wait(nullptr);
        }
    }

    return 0;
}
