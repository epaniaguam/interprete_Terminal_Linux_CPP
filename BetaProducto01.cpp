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

void reconocerHomeUser(string &ruta)
{
  if (ruta[0] == '~')
  {
    const char *homeDir = getenv("HOME"); // Obtener el directorio de inicio del usuario

    if (homeDir == nullptr)
    {
      cerr << "No se pudo obtener el directorio de inicio del usuario." << endl;
    }
    else
    {
      ruta = ruta.substr(1, ruta.length());
      ruta = string(homeDir) + ruta;
    }
  }
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

bool existeRedireccionamiento(string &input, string &rutaRedireccionamiento)
{
  rutaRedireccionamiento = "";
  int pos = input.find('>');
  if (pos != -1)
  {
    // Separamos el redireccionamiento
    rutaRedireccionamiento = input.substr(pos + 2, input.length());
    if (input[0] == ' ')
    {
      input.erase(pos - 1, input.length());
    }
    else
    {
      input.erase(pos, input.length());
    }

    ////////////////////////
    return true;
  }
  else
  {
    rutaRedireccionamiento = "";
    return false;
  }
}

void extraerOpcionesArgumentos(string comando_general, vector<string> &opciones, vector<string> &argumentos)
{
  // Analizar el comando
  int pos = 0;
  while ((pos = comando_general.find(' ')) != -1)
  {
    string elemento = comando_general.substr(0, pos);
    // Identificar si es el comando, opción o argumento
    if (elemento[0] == '-')
    {
      opciones.push_back(elemento);
    }
    else
    {
      argumentos.push_back(elemento);
    }
    comando_general.erase(0, pos + 1);
  }
  // Obtener el ultimo argumento
  if (comando_general != "")
  {
    argumentos.push_back(comando_general);
  }

  // Identificar si se uso "~" para el directorio de inicio del usuario
  for (int i = 0; i < argumentos.size(); i++)
  {
    reconocerHomeUser(argumentos[i]);
  }
}

int main()
{
  while (true)
  {
    string comando_general;

    /************** MOSTRAR LA RUTA ACTUAL Y ESPERAR EL COMANDO DEL USUARIO ****************/
    inprimirTerminal();

    getline(cin, comando_general);

    if (comando_general == "salir")
    {
      break;
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

    // Comprobar que existe y Separar el redireccionamiento
    existeRedireccion = existeRedireccionamiento(comando_general, rutaRedireccionamiento);

    // Identificar si se uso "~" para el directorio de inicio del usuario
    reconocerHomeUser(rutaRedireccionamiento);

    // Extraer opciones y argumentos
    extraerOpcionesArgumentos(comando_general, opciones, argumentos);

    elementoscmd.insert(elementoscmd.end(), opciones.begin(), opciones.end());
    elementoscmd.insert(elementoscmd.end(), argumentos.begin(), argumentos.end());

    /******************************* FIN DE LA EXTRACCIÓN  ********************************/
    /************************* AGREGAR LOS ARGUMENTOS PARA EXECV **************************/
    char **args = nullptr;

    // Número inicial de argumentos
    int num_args = 0;

    // Añadir elementos iterativamente desde el vector de strings
    for (const auto &str : elementoscmd)
    {
      args = static_cast<char **>(realloc(args, (num_args + 1) * sizeof(char *)));
      args[num_args] = strdup(str.c_str());
      num_args++;
    }

    // Añadir el último elemento nulo
    args = static_cast<char **>(realloc(args, (num_args + 1) * sizeof(char *)));
    args[num_args] = nullptr;
    num_args++;

    /******************************* FIN ARGUMENTOS PARA EXECV  ********************************/
    /******************************* EJECUCION DE PROGRAMA  ******************************/
    if (existeRedireccion)
    {
      int pidRedireccion = fork();
      if (pidRedireccion == 0)
      { // proceso hijo para la redirección
        // Configura la redirección
        FILE *outputFile = freopen(rutaRedireccionamiento.c_str(), "w", stdout);
        if (outputFile == nullptr)
        {
          perror("freopen");
          return 1;
        }

        ////// Ejecuta el comando //////
        // Llamar a execv con el array de argumentos
        execv(rutaComando.c_str(), args);
        // Si execv retorna, es porque hubo un error
        perror("execv");
        //////
        return 1;
      }
      else if (pidRedireccion > 0)
      {
        // Proceso padre
        waitpid(pidRedireccion, NULL, 0); // Espera a que el hijo termine
      }
      else
      {
        perror("fork");
        return 1;
      }
    }
    else
    {
      ////// Ejecuta el comando //////
      int pid = fork();
      if (pid == 0)
      { // Proceso hijo
        // Ejecuta el comando
        // Llama a execv con el array de argumentos
        execv(rutaComando.c_str(), args);
        // Si execv retorna, es porque hubo un error
        perror("execv");
        return 1;
      }
      else if (pid > 0)
      {
        // Proceso padre
        waitpid(pid, NULL, 0); // Espera a que el hijo termine
      }
      else
      {
        perror("fork");
        return 1;
      }
      //////
    }
  }
  return 0;
}
