//char * readline (const char *prompt);

#include <readline/readline.h>
#include <readline/history.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void command(char *argv[]);

static char *line = {""};

int main(){
  int e;
  char *envp[] = { NULL };
  char *argv[] = { "" };
  char *token;
  pid_t pid;

  while (strncmp("exit", line, 4) != 0){
    command(argv);
    
    if (argv[0] != ""){
      if ((pid = fork()) == 0)
	e = execve(argv[0], argv, envp);
        
      else
	waitpid(-1, NULL, 0);
    }
  }

  free(line);
  return 0;
}


void command(char *argv[]){
  const char *cwd = (const char *) get_current_dir_name();      
  const char *ex;
  char *token;
  char prompt[70] = "[";
  int i;

  //TRATAMENTO DO PROMPT [local]
  ex = strtok((char *)cwd, " ");
  strcat(prompt, ex);
  strcat(prompt, "] ");

  line = readline(prompt);

  if (line && *line) 
    add_history(line);

  argv[0] = "";
  token = strtok(line, " ");

  if (token == NULL) 
    return;

  //TRATAMENTO DE ls
  else  if (strcmp(token, "ls") == 0){
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
    printf("%s\n",cwd);

  free((char *)cwd);
}
