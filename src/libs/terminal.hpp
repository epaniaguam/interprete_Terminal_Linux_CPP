
// #include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstring>
#include <filesystem>
#include "comando.hpp"

#define ESTATUS int
// ESTADOS
#define ERROR 1
#define BREAK 2
#define CONTINUE 3

using namespace std;

class Terminal
{
public:
  // Atributos
  string user;
  // Constructor
  Terminal();
  // Metodos
  vector<string> directorio_actual();
  void completarRuta(string &);
  void inprimirPrompt();
  int ejecutarComando(Comando);
};

Terminal::Terminal()
{
  vector<string> dir = directorio_actual();
  user = dir[2];
};

int Terminal::ejecutarComando(Comando comando)
{
  if (comando.comando_principal == "salir")
  {
    return BREAK;
  }
  else if (comando.comando_principal == "limpiar")
  {
    system("clear");
  }
  else if (comando.comando_completo == "")
  {
    return CONTINUE;
  }
  else if (comando.comando_principal == "cd")
  {
    completarRuta(comando.argumentos[0]);
    if (chdir(comando.argumentos[0].c_str()) != 0)
    {
      perror("Error al cambiar el directorio");
    }
    return CONTINUE;
  }
  else if (comando.redireccionamiento == ">")
  {
    completarRuta(comando.archivoSalida);

    if(comando.usarPipe){
      int pipefd[2];
      pipe(pipefd); // Crear la tubería

      int pid = fork(); // Crear un nuevo proceso
      if (pid == 0)
      {
        // Código del proceso hijo
        // Redirigir la salida estándar al extremo de escritura de la tubería
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);  // Cerrar el extremo de lectura de la tubería

        // Ejecutar el primer comando
        execv(comando.comand_path(), comando.comand_argv());
        perror("execv");
        return ERROR;
      }
      else
      {
        // Código del proceso padre
        // Cerrar el extremo de escritura de la tubería en el proceso padre
        close(pipefd[1]);

        // Esperar a que el proceso hijo termine
        waitpid(pid, NULL, 0);

        // Ahora puedes realizar la redirección de entrada para el siguiente comando (segundo comando)
        // (Este código es un ejemplo, necesitarás ajustarlo según tus necesidades)
        int stdin_original = dup(STDIN_FILENO);  // Guardar el descriptor de archivo original

        // Redirigir la entrada estándar al extremo de lectura de la tubería
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);  // Cerrar el extremo de lectura de la tubería

        // Crear un nuevo proceso para el segundo comando
        pid_t pid2 = fork();

        if (pid2 == -1) {
        perror("fork");
        return ERROR;
        }

        if (pid2 == 0) {
          // Código del segundo proceso hijo
          // Ejecutar el segundo comando
          FILE *outputFile = freopen(comando.archivoSalida.c_str(), "w", stdout);
          if (outputFile == nullptr)
          {
            perror("freopen");
            return ERROR;
          }
          execv(comando.ruta_comandoPipe.c_str(), comando.comand_argvPipe());
          perror("execv");
          return ERROR;
        } else {
            // Código del proceso padre
            // Restaurar la entrada estándar al descriptor original
            dup2(stdin_original, STDIN_FILENO);
            close(stdin_original);

            // Esperar a que el segundo proceso hijo termine
            waitpid(pid2, NULL, 0);
        }
      }
    } else {
      int pidRedireccion = fork();
      if (pidRedireccion == 0)
      {
        FILE *outputFile = freopen(comando.archivoSalida.c_str(), "w", stdout);
        if (outputFile == nullptr)
        {
          perror("freopen");
          return ERROR;
        }
        execv(comando.comand_path(), comando.comand_argv());
        perror("execv");
        return ERROR;
      }
      else if (pidRedireccion > 0)
      {
        if (!comando.segundoplano){
          waitpid(pidRedireccion, NULL, 0);
        }else{
          cout << "Proceso en segundo plano: " << pidRedireccion << endl;
        }
      }
      else
      {
        perror("fork");
        return ERROR;
      }
    }


  }
  else
  {
    if(comando.usarPipe){
      int pipefd[2];
      pid_t pid;

      // Creamos la tuberia
      if (pipe(pipefd) == -1) {
        perror("pipe");
        return ERROR;
      }

      // Creamos el proceso hijo
      pid = fork();
      if (pid == -1) {
        perror("fork");
        return ERROR;
      }

      if (pid == 0)
      {
        // Código del proceso hijo
        // Redirigir la salida estándar al extremo de escritura de la tubería
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);  // Cerrar el extremo de lectura de la tubería

        // Ejecutar el primer comando
        execv(comando.comand_path(), comando.comand_argv());
        perror("execv");
        return ERROR;
      } else {
        // Código del proceso padre
        // Cerrar el extremo de escritura de la tubería en el proceso padre
        close(pipefd[1]);

        // Esperar a que el proceso hijo termine
        waitpid(pid, NULL, 0);

        // Ahora puedes realizar la redirección de entrada para el siguiente comando (segundo comando)
        // (Este código es un ejemplo, necesitarás ajustarlo según tus necesidades)
        int stdin_original = dup(STDIN_FILENO);  // Guardar el descriptor de archivo original

        // Redirigir la entrada estándar al extremo de lectura de la tubería
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);  // Cerrar el extremo de lectura de la tubería

        // Crear un nuevo proceso para el segundo comando
        pid_t pid2 = fork();

        if (pid2 == -1) {
        perror("fork");
        return ERROR;
        }

        if (pid2 == 0) {
        // Código del segundo proceso hijo
        // Ejecutar el segundo comando
        execv(comando.ruta_comandoPipe.c_str(), comando.comand_argvPipe());
        perror("execv");
        return ERROR;
        } else {
            // Código del proceso padre
            // Restaurar la entrada estándar al descriptor original
            dup2(stdin_original, STDIN_FILENO);
            close(stdin_original);

            // Esperar a que el segundo proceso hijo termine
            waitpid(pid2, NULL, 0);
        }

      }

    } else {
      int pid = fork();
      if (pid == 0)
      {
        execv(comando.comand_path(), comando.comand_argv());
        perror("execv");
        return ERROR;
      }
      else if (pid > 0)
      {
        if (!comando.segundoplano){
          waitpid(pid, NULL, 0);
        }else{
          cout << "Proceso en segundo plano: " << pid << endl;
        }
      }
      else
      {
        perror("fork");
        return ERROR;
      }
    }

  }
  return 0;
}

vector<string> Terminal::directorio_actual()
{
  filesystem::path currentPath = filesystem::current_path();
  vector<string> directories;
  for (const auto &dir : currentPath)
    directories.push_back(dir.string());
  return directories;
}

void Terminal::inprimirPrompt()
{
  vector<string> directories = directorio_actual();
  int pos = directories.size();
  cout << BLUE << user << YELLOW << "@" << GREEN_LIGHT << "ESIS" << WHITE << ":" << PURPLE;
  if (directories.size() < 3)
  {
    for (int i = 0; i < pos; i++)
      cout << "/" << directories[i];
  }
  else
  {
    cout << "~";
    for (int i = 3; i < pos; i++)
      cout << "/" << directories[i];
  }
  cout << YELLOW << "$ " << SKYBLUE_LIGHT;
}

void Terminal::completarRuta(string &ruta)
{
  if (ruta[0] == '~')
  {
    const char *homeDir = getenv("HOME"); // Obtener el directorio de inicio del usuario
    if (homeDir != nullptr)
    {
      ruta = ruta.substr(1, ruta.length());
      ruta = string(homeDir) + ruta;
    }
    else
    {
      cerr << "No se pudo obtener el directorio de inicio del usuario." << endl;
    }
  }
}