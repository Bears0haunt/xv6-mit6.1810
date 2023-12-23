#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//exec 
void run(char *program, char **args) {
	if(fork() == 0) { // child exec
		exec(program, args);
		exit(0);
	}
	return; // parent return
}

int main(int argc, char *argv[]){
	char buf[2048]; // buffer
	char *p = buf, *last_p = buf; // start and end pointer
	char *argsbuf[128]; // the all argurments argv[] and stdin
	char **args = argsbuf; // the first stdin argument

	for(int i=1;i<argc;i++) {
		*args = argv[i];
		args++;
	}

	char **pa = args; // 开始读入参数
	while(read(0, p, 1) != 0) {
		if(*p == ' ' || *p == '\n') {
			// an argument read done
			*p = '\0';	
			*(pa++) = last_p;
			last_p = p+1;

			if(*p == '\n') {
				// readline done
				*pa = 0;
				run(argv[1], argsbuf); 
				pa = args; //reset to read next line
			}
		}
		p++;
	}

	if(pa != args) {  // if last line not null
		*p = '\0';
		*(pa++) = last_p;
		*pa = 0; 
    //exec the last line 
		run(argv[1], argsbuf);
	}
	while(wait(0) != -1) {}; // wait child 
	exit(0);
}