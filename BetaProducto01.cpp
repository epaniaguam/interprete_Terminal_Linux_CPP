#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstring>
#include <filesystem>

// Definición de códigos de colores ANSI
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define SKYBLUE "\033[36m"
#define WHITE "\033[37m"
#define GREEN_LIGHT "\033[92m"

using namespace std;

void inprimirTerminal()
{
  filesystem::path currentPath = filesystem::current_path();

  auto it = currentPath.begin();
  advance(it, 2);

  cout << PURPLE << it->string() << "@ESIS" << WHITE << ":" << YELLOW << " $ " << SKYBLUE; //<< RESET
}

string comprobarRutaComando(string &input, string &cmd)
{
  string rutaComando = "/bin/";
  int pos;
  if (input[0] == '/')
  {
    // Separamos la ruta del comando
    pos = input.find(' ');
    rutaComando = input.substr(0, pos);
    input.erase(0, pos + 1);
    ////////////////////////

    // Verificamos si en la ruta se incluyo el commando, ejemplo >> /bin/ls ls o puede que no >>  /bin ls
    pos = input.find(' ');
    string comando = input.substr(0, pos);
    string lastLetters = rutaComando.substr(rutaComando.find_last_of("/") + 1);

    // Comprobar existencia y añadir el comando a la ruta
    if (lastLetters != comando)
    {
      if (rutaComando.back() == '/')
        rutaComando += comando;
      else
        rutaComando += "/" + comando;
    }
    ////////////////////////

    // Extraer el comando del input
    cmd = comando;
    if (pos == -1)
    {
      input.erase(0, pos);
    }
    else
    {
      input.erase(0, pos + 1);
    }
  }
  else
  {

    // Verificamos si en la ruta se incluyo el commando, ejemplo >> /bin/ls ls   o puede que no >>  /bin ls
    pos = input.find(' ');
    cmd = input.substr(0, pos);
    string lastLetters = rutaComando.substr(rutaComando.find_last_of("/") + 1);

    // Comprobar existencia y añadir el comando a la ruta
    if (lastLetters != cmd)
    {
      if (rutaComando.back() == '/')
        rutaComando += cmd;
      else
        rutaComando += "/" + cmd;
    }

    // Si no se incluyo la ruta del comando
    pos = input.find(' ');
    if (pos == -1)
    {
      cmd = input;
      input = "";
    }
    else
    {
      cmd = input.substr(0, pos);
      input.erase(0, pos + 1);
    }
  }
  return rutaComando;
}

int main()
{
  string comando_general;

  /************** MOSTRAR LA RUTA ACTUAL Y ESPERAR EL COMANDO DEL USUARIO ****************/
  inprimirTerminal();

  getline(cin, comando_general);

  if (comando_general == "salir")
  {
    // break;
  }

  string rutaComando, cmd, rutaRedireccionamiento;
  bool existeRedireccion;
  vector<string> opciones;
  vector<string> argumentos;
  vector<string> elementoscmd;

  /*********** EXTRAER LA INFORMACION DEL COMMANDO INGRESADO POR EL USUAIRIO ************/

  // Extraer ruta (si existe) y extraer el comando
  rutaComando = comprobarRutaComando(comando_general, cmd);
  // elementoscmd.push_back(rutaComando);
  elementoscmd.push_back(cmd);
  return 0;
}
