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

int main()
{
  inputComando = "/bin/ cp -r /directorio /copia_directorio > ~/Dodumentos/texto.txt";
  string inputComando;
  string modoSudo;
  modoSudo = extraerSudo(inputComando); // aun sin usar
  
}
