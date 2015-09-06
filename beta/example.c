#include <stdio.h>
void 

int main() {
	
  while(1) {
    type_prompt();
    read_command(command, parameters);
    
    if (fork() != 0)
      waitpid(-1,&);
    else
      execve(command, par, 0);/*0 é variável de ambiente*/
  }
  
	execve()  
  return 0;
}
