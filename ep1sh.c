//char * readline (const char *prompt);

#include <readline/readline.h>
#include <readline/history.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


void firstCmd(char *argv[]);
void prompt();
static char *line = (char *)NULL;


int main(){
  int e;
  char *envp[] = { NULL };
  char *argv[] = { "" };
  char *token;

  const char str1[10] = {"exit"};
  pid_t pid;
  
  prompt();
  line = readline("");

  while (strcmp(line, str1) != 0){

    firstCmd(argv);

    
    if(argv[0] != NULL && argv[0] != "" ){
      if ((pid = fork()) == 0)
	e = execve(argv[0], argv, envp);
        
      else

	waitpid(-1, NULL, 0);
    }

    if (line && *line)
      add_history(line);


    argv[0] = NULL;
    prompt();
    line = readline("");
  }

  free(line);
  return 0;
}


void prompt() {
  const char *cwd = (const char *) get_current_dir_name();      
  printf("[%s] ",cwd);

}

void firstCmd(char *argv[]){
  char *token;
  char *ex;
  int i;

  char *cwd;

  token = strtok(line, " ");

  //TRATAMENTO DE ls
  if (strcmp(token, "ls") == 0){
    argv[0] = "/bin/ls";

    token = strtok(NULL, " ");

    for (i = 1; token != NULL; i++){
      argv[i] = token;
      token = strtok(NULL, " ");
    }
    argv[i] = NULL;
  }

  //TRATAMENTO DE cd
  else if (strcmp(token, "cd") == 0) {
    token = strtok(NULL, " ");
    chdir(token);
  }

  //TRATAMENTO DE pwd
  else if (strcmp(token, "pwd") == 0)
    printf("%s\n\n", (const char *)get_current_dir_name());

  else
    argv[0] = NULL;
}
