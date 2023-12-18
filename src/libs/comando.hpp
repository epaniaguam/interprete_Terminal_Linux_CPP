#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// Definición de códigos de colores ANSI
#define RESETEAR "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define SKYBLUE "\033[36m"
#define WHITE "\033[37m"
#define GREEN_LIGHT "\033[92m"
#define SKYBLUE_LIGHT "\033[96m"

using namespace std;

class Comando
{
public:
  // Atributos
  string comando_completo;
  // Comando completo ingresado por el usuario
  string ruta_comando;
  // Desestructuracion del comando
  vector<string> linea_comando;

  string redireccionamiento;
  string archivoEntrada;
  string archivoSalida;

  bool usarPipe;

  string comando_principal;
  vector<string> opciones;
  vector<string> argumentos;

  // Constructor
  Comando(string &);

  // Metodo para imprimir los atributos del comando
  void imprimir();
  const char *comand_path();
  char *const *comand_argv();
  void completarRuta(string &);
};

void Comando::completarRuta(string &ruta)
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

Comando::Comando(string &cmd) : usarPipe(false)
{
  // Analizar el comando y extraer el nombre, argumentos y redirecciones
  istringstream iss(cmd);
  comando_completo = cmd;
  string token;

  while (iss >> token)
  {
    if (token == "<")
    {
      redireccionamiento = token;
      iss >> archivoEntrada;
    }
    else if (token == ">" || token == ">>")
    {
      redireccionamiento = token;
      iss >> archivoSalida;
    }
    else if (token == "|")
    {
      usarPipe = true;
    }
    else
    {
      if (linea_comando.size() == 0)
        comando_principal = token;
      else if (token[0] == '-')
        opciones.push_back(token);
      else
        argumentos.push_back(token);
      completarRuta(token);
      linea_comando.push_back(token);
    }
  }
  if (comando_principal[0] == '/')
  {
    ruta_comando = comando_principal;
    comando_principal = comando_principal.substr(comando_principal.find_last_of("/") + 1);
  }
  else
  {
    ruta_comando = "/bin/" + comando_principal;
  }
}

void Comando::imprimir()
{
  cout << SKYBLUE << "Comando: " << WHITE << comando_completo << endl;
  cout << SKYBLUE << "Ruta: " << WHITE << ruta_comando << endl;
  cout << SKYBLUE << "Comando principal: " << WHITE << comando_principal << endl;
  cout << SKYBLUE << "Opciones: " << WHITE;
  for (auto &opcion : opciones)
    cout << opcion << " ";
  cout << endl;
  cout << SKYBLUE << "Argumentos: " << WHITE;
  for (auto &argumento : argumentos)
    cout << argumento << " ";
  cout << endl;
  cout << SKYBLUE << "Redireccionamiento: " << WHITE << redireccionamiento << endl;
  cout << SKYBLUE << "Archivo de entrada: " << WHITE << archivoEntrada << endl;
  cout << SKYBLUE << "Archivo de salida: " << WHITE << archivoSalida << endl;
  cout << SKYBLUE << "Usar pipe: " << WHITE << usarPipe << endl;

  cout << SKYBLUE << "Linea de comando: " << WHITE;
  for (auto &elemento : linea_comando)
    cout << elemento << " ";
  cout << endl;
}

const char *Comando::comand_path()
{
  return ruta_comando.c_str();
}

char *const *Comando::comand_argv()
{
  int nargs = linea_comando.size();
  const char *arrayDeArgumentos[nargs + 1];
  for (int i = 0; i < nargs; ++i)
    arrayDeArgumentos[i] = linea_comando[i].c_str();
  arrayDeArgumentos[nargs] = nullptr;
  char *const *argv = const_cast<char *const *>(arrayDeArgumentos);
  return argv;
}