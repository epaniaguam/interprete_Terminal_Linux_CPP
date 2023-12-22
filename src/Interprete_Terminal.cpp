
#include "libs/terminal.hpp"

int main()
{
  Terminal term;
  while (true)
  {
    term.inprimirPrompt();

    string comando_general;
    getline(cin, comando_general);

    Comando cmd(comando_general);

    ESTATUS estado = term.ejecutarComando(cmd);

    if (estado == ERROR)
      return ERROR;
    else if (estado == BREAK)
      break;
    else if (estado == CONTINUE)
      continue;
  }

  /* string comando_general;
  getline(cin, comando_general);

  Comando cmd(comando_general);

  cmd.imprimir();
 */
  return 0;
}
