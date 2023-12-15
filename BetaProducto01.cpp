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
    string modoSudo,ruta,comando, argumentos,redireccion,archivo, rutaComando, dosArgumentos,argOrigen,argDestino;
    modoSudo = extraerSudo(inputComando); // aun sin usar
    ruta = extraerRuta(inputComando);
        
    dosArgumentos = dosArgumentosConEspacio(inputComando); //funcion para capturar dos argumentos con espacio
  
}
