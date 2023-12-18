
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
      waitpid(pidRedireccion, NULL, 0);
    }
    else
    {
      perror("fork");
      return ERROR;
    }
  }
  else
  {
    int pid = fork();
    if (pid == 0)
    {
      execv(comando.comand_path(), comando.comand_argv());
      perror("execv");
      return ERROR;
    }
    else if (pid > 0)
    {
      waitpid(pid, NULL, 0);
    }
    else
    {
      perror("fork");
      return ERROR;
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