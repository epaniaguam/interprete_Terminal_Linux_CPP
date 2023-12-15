#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <regex>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

string extraerSudo(string &input)
{
    string modoSudo = "";
    if (input.find("sudo") == 0)
    {
        modoSudo = "sudo";
        input = input.substr(5, input.length());
    }
    return modoSudo;
}
string extraerRuta(string &input)
{
    string ruta = "/bin/";
    //regex expruta("^(~|\\.|\\/)+(?:[a-zA-Z0-9_-]+\\/?)+"); // Expresión regular
    regex expruta("^(~|\\.|\\/)+(?:[a-zA-Z0-9_-]+\\/?)+(\\.[a-zA-Z0-9]+)*");  //Expresion mejorada aun para poner a prueba
    smatch match;                        // variable de regex para almacenar la coincidencia

    bool coincide_ruta = regex_search(input, match, expruta);

    if (coincide_ruta)
    {
        ruta = match.str(); // Extrae la ruta encontrada
        if(input.length() > ruta.length())
            input = input.substr(ruta.length()+1, input.length());
        else
            input = input.substr(ruta.length(), input.length());
    }
    return ruta;
}

string extraerRutacomando(string &input)
{
    string ruta = "";
    //regex expruta("^(~|\\.|\\/)+(?:[a-zA-Z0-9_-]+\\/?)+"); // Expresión regular
    regex expruta("^(~|\\.|\\/)+(?:[a-zA-Z0-9_-]+\\/?)+(\\.[a-zA-Z0-9]+)*");  //Expresion mejorada aun para poner a prueba
    smatch match;                        // variable de regex para almacenar la coincidencia

    bool coincide_ruta = regex_search(input, match, expruta);

    if (coincide_ruta)
    {
        ruta = match.str(); // Extrae la ruta encontrada
        if(input.length() > ruta.length())
            input = input.substr(ruta.length()+1, input.length());
        else
            input = input.substr(ruta.length(), input.length());
    }else{
        int posRedir = input.find(">");
        if(posRedir != string::npos){
            ruta = input.substr(0,posRedir-1);
            input = input.substr(posRedir,input.length());
        }else{
            ruta = input;
            input = "";
        }
    }
    return ruta;
}
string extraerComando(string &input)
{
    string comando = "";
    if(input == "")
        return comando;
    else {
        regex exp("^[a-zA-Z]+"); // Expresión regular
        smatch match;        // variable de regex para almacenar la coincidencia

        bool coincide = regex_search(input, match, exp);

        if (coincide)
        {
            comando = match.str(); // Extrae la ruta encontrada
            if(input.length() > comando.length())
                input = input.substr(comando.length()+1, input.length());
            else if(input.length() == comando.length())
                input = "";
        }
        return comando;
    }
}
string extraerArgumentos(string &input)
{
    // Ejemplos de como se veria un comando
        // "ls -lsa /home/ > salida.txt"
        // "ls -lsa ./home/ > salida.txt"
        // "ls -lsa ~/Descargas/ > ~/Descargas/salida.txt"
    
    // Buscar la primera aparición de "/" o "./" o "~"
    int found1 = input.find("/");
    int found2 = input.find("../");
    int found3 = input.find("./");
    int found4 = input.find("~");
    int found5 = input.find(">");
    
    // Encontrar el índice de la primera aparición
    int posterminoClave = string::npos; // Valor predeterminado
    
    if (found1 != string::npos)
        posterminoClave = found1;
    // verificamos si se encontro found2 y que aparesca antes de found1
    if (found2 != string::npos && (posterminoClave == string::npos || found2 < posterminoClave))
        posterminoClave = found2;
    // igual que en el caso anterior
    if (found3 != string::npos && (posterminoClave == string::npos || found3 < posterminoClave))
        posterminoClave = found3;
    // igual que en el caso anterior
    if (found4 != string::npos && (posterminoClave == string::npos || found4 < posterminoClave))
        posterminoClave = found4;
    if (found5 != string::npos && (posterminoClave == string::npos || found5 < posterminoClave))
        posterminoClave = found5;


    string argumentos,subcadena;
    argumentos = "";
    if(input[0] != '-')
        return argumentos;

    regex expresion("(^\\s+)|(\\s+$)");

    if (posterminoClave != string::npos) {
        // Si se encuentra "/" o "./" o "~" o ">", extraer los argumentos antes de.

        subcadena = input.substr(0, posterminoClave);
        argumentos = regex_replace(subcadena, expresion, ""); // elimina espacios antes y despues de la cadena
        
        input = input.substr(posterminoClave, input.length());

    } else {
        // Si no se encuentra terminos clave, toma la cadena completa
        argumentos = input;
    }
    return argumentos;

}
void extraerRedireccion(string &input,string &redireccion, string &archivo){
    if(input != ""){
        redireccion = input.substr(0,1);
        input = input.substr(1,input.length());
        if(input.length() >= 1 && input[0] == ' ') //verifica si existe mas luego de la redireccion y elimina el espacio
            archivo = input.substr(1,input.length());
    }
}

void reconocerHomeUser(string &ruta){
    if(ruta[0] == '~'){
        const char* homeDir = getenv("HOME"); // Obtener el directorio de inicio del usuario
        
        if (homeDir == nullptr) {
            cerr << "No se pudo obtener el directorio de inicio del usuario." << endl;
        }else{

            ruta = ruta.substr(1,ruta.length());
            ruta = string(homeDir) + ruta;
        }
    }
}
string dosArgumentosConEspacio(string &input){
 
    string expdirectorio = "(~|(\\.\\/)|\\/)+(?:[a-zA-Z0-9_-]+\\/?)+";
    //string expdirectorio = "\"*((~|(\\.\\/)|\\/)+(?:[a-zA-Z0-9_-]+\\/?)+)*"; //considera comillas, sin pruebas aun 100% fiables aun
    string exparchivo = "(~|\\.|\\/)*(?:[a-zA-Z0-9_-]+\\/?)+(\\.[a-zA-Z0-9]+)+";
    //string exparchivo = "\"*((~|\\.|\\/)*(?:[a-zA-Z0-9_-]+\\/? *)+(\\.[a-zA-Z0-9]+)+)\"*"; //considerada comillas, sin pruebas aun 100% fiables aun

    // Unir las expresiones regulares en una expresión más grande
    string dirTodir = expdirectorio + " " + expdirectorio;
    string dirToarch = expdirectorio + " " + exparchivo;
    string archToarch = exparchivo + " " + exparchivo;
    string archTodir = exparchivo + " " + expdirectorio;smatch match1;
    
    // Crear una expresión regular compuesta
    regex regexPattern("(" + dirTodir + ")|(" + dirToarch + ")|(" + archToarch + ")|(" + archTodir + ")");

    // Buscar una coincidencia en el input
    smatch match;
    if (regex_search(input, match, regexPattern)) {
        // Encontró una coincidencia
        string coincidencia = match.str(0); // Obtener la coincidencia
        input = regex_replace(input, regexPattern, ""); // Eliminar la coincidencia del input
        
        int dobleespacio = input.find("  ");
        if(dobleespacio != string::npos)
            input.replace(dobleespacio,2," "); // Eliminar espacios dobles
        return coincidencia;
    } else {
        // No se encontraron coincidencias
        return "";
    }
}



int main()
{
    string inputComando;
    
    //inputComando = "/bin/ ls -lsa / > ~/Descargas/salida.txt";
    //inputComando = "/bin/ ls ~/Descargas/";
    //inputComando = "/bin/ rm ~/Descargas/pdf-msx88_compress.pdf";
    //inputComando = "clear";
    inputComando = "/bin/ cp -r /directorio /copia_directorio > ~/Dodumentos/texto.txt";
    //inputComando = "rm 11.txt";


    //cout << "▒ESIS $ ";
    //getline(cin,inputComando);
    
    string modoSudo,ruta,comando, argumentos,redireccion,archivo, rutaComando, dosArgumentos,argOrigen,argDestino;
    modoSudo = extraerSudo(inputComando); // aun sin usar
    ruta = extraerRuta(inputComando);
        
    dosArgumentos = dosArgumentosConEspacio(inputComando); //funcion para capturar dos argumentos con espacio
    argOrigen = dosArgumentos.substr(0,dosArgumentos.find(" "));
    argDestino = dosArgumentos.substr(dosArgumentos.find(" ")+1,dosArgumentos.length());


    comando = extraerComando(inputComando);
    argumentos = extraerArgumentos(inputComando);
    rutaComando = extraerRutacomando(inputComando);
    reconocerHomeUser(rutaComando);

    redireccion = ""; archivo = "";
    //captura redireccion y archivo si es que hay esos elementos en el inputComando
    //caso contrario los mantiene en vacio
    extraerRedireccion(inputComando,redireccion,archivo); 
    reconocerHomeUser(archivo);

    if (redireccion == ">" && archivo != "") {
        int pidRedireccion = fork();
        if (pidRedireccion == 0) { // proceso hijo para la redirección
            // Configura la redirección
            FILE* outputFile = freopen(archivo.c_str(), "w", stdout);
            if (outputFile == nullptr) {
                perror("freopen");
                return 1;
            }

            // Ejecuta el comando
            ruta += comando;

            if(!rutaComando.empty() && !argumentos.empty()){
                execl(ruta.c_str(), comando.c_str(), argumentos.c_str(),rutaComando.c_str(), NULL);
            }else if (!argumentos.empty() && !dosArgumentos.empty()) {
                execl(ruta.c_str(), comando.c_str(), argumentos.c_str(), argOrigen.c_str(),argDestino.c_str(), NULL);
            }else if (!dosArgumentos.empty()) {
                execl(ruta.c_str(), comando.c_str(), argOrigen.c_str(),argDestino.c_str(), NULL);
            }else if (!argumentos.empty()) {
                execl(ruta.c_str(), comando.c_str(), argumentos.c_str(), NULL);
            }else if (!rutaComando.empty()){
                execl(ruta.c_str(), comando.c_str(), rutaComando.c_str(), NULL);
            } else {
                execl(ruta.c_str(), comando.c_str(), NULL);
            }

            perror("execl");
            return 1;
        } else {
            waitpid(pidRedireccion, NULL, 0); // Espera a que el hijo termine
        }
    } else {
        // Ejecuta el comando sin redirección
        ruta += comando;
        int pid = fork();
        if (pid == 0) { // proceso hijo
            if(!rutaComando.empty() && !argumentos.empty()){
                execl(ruta.c_str(), comando.c_str(), argumentos.c_str(),rutaComando.c_str(), NULL);
            }else if (!argumentos.empty() && !dosArgumentos.empty()) {
                execl(ruta.c_str(), comando.c_str(), argumentos.c_str(), argOrigen.c_str(),argDestino.c_str(), NULL);
            }else if (!dosArgumentos.empty()) {
                execl(ruta.c_str(), comando.c_str(), argOrigen.c_str(),argDestino.c_str(), NULL);
            }else if (!argumentos.empty()) {
                execl(ruta.c_str(), comando.c_str(), argumentos.c_str(), NULL);
            }else if (!rutaComando.empty()){
                execl(ruta.c_str(), comando.c_str(), rutaComando.c_str(), NULL);
            } else {
                execl(ruta.c_str(), comando.c_str(), NULL);
            }
            perror("execl");
            return 1;
        } else {
            wait(0);
        }
    }


}
