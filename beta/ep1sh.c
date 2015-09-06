/******************************************************
 * EP1SH : Sistemas Operacionais
 * Florence Alyssa Sakuma Shibata		NUSP
 * Shayenne da Luz Moura						NUSP 8536235
 ******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

static char *line_read = (char *) NULL;

static char *buf;

void type_prompt() {

 
    rl_bind_key('\t',rl_abort);//disable auto-complete
 
    // Deixar o prompt bonitinho []
    const char *prompt = (const char *) get_current_dir_name();

    buf = readline("[",prompt,"]"));
    
        if (strcmp(buf,"quit")==0)
            break;
 
        printf("[%s]\n",buf);
 
        if (buf[0]!=0)
            add_history(buf);    
}

int main() {
	while (1) {
		type_prompt();
		//read_command(command, parameters);
		if (fork() != 0) {
		/* Codigo do pai */
			waitpid(-1,&status,0);
		} else {
			printf("OI\n");
		/* Codigo do filho */
			//execve(command,parameters,0);
	}
	
	free(buf);
	return 0;
	
}
}
