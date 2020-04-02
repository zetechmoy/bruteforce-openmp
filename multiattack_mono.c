// multiattack.c
// Starts multiple instances checking if a password can be found in the dictionnary.
// usage : nb_of_processus dictionnary_file shasum_file


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

// Default maximum number of simultaneous process
int MAX_FILS = 5;

//wraup for readline
char * readline(FILE * f){
	char * line = NULL;

	size_t len = 0;
	ssize_t read;
	if ((read = getline(&line, &len, f)) != -1) {
		line[read-2] = '\0';

		return line;
	}

	return NULL;
}

void write_file(char * filename, int content){
	FILE * fp;
	/* open the file for writing*/
	fp = fopen (filename, "a");
	fprintf(fp, "%d", content);
	fprintf(fp, ",");
	fprintf(fp, "%ld", time(NULL));
	fprintf(fp, "\n");

	/* close the file*/
	fclose (fp);
}


int main(int argc, char *argv[]) {
	if(argc < 2) fprintf(stderr, "Usage: '%s' nb_of_processus dictionnary_file shasum_file\n", argv[0]), exit(EXIT_FAILURE);

	char *p;
	MAX_FILS = strtol(argv[1], &p, 10);
	char * dict_file = argv[2];
	char * shasum_file = argv[3];


	printf("MAX_FILS = %d\n", MAX_FILS);
	printf("dict_file = %s\n", dict_file);
	printf("shasum_file = %s\n", shasum_file);

	// opening file
	FILE * ds = fopen(shasum_file, "r");
	if (ds == NULL) exit(EXIT_FAILURE);

	char * current_password_to_analyse = readline(ds);
	int current_checker_running = 0;

	int child_pid = 0;
	while (current_password_to_analyse!=NULL){
		for ( ; current_checker_running < MAX_FILS ; ){ // forking until reaching MAX_FILS sons
			if ((child_pid=fork())<0){
				perror("fork error");
				exit(1);
			}
			if (child_pid!=0) {
				//Father code, so I'm couting one more son

				current_checker_running++;
				//printf("\n[INFO] Started  %dth son searching for password %s\n", current_checker_running, current_password_to_analyse);
				current_password_to_analyse = readline(ds);

			} else { // son code.
				//write_file("son_out.txt", counter+1);
				execl("/bin/grep", "grep", "--binary-files=text", current_password_to_analyse, dict_file, NULL);
				// There's no return from execl, remember it ?
			}

		} // for end. So I should have more or less MAX_FILS sons, until one's returning ...

		pid_t any_child;
		wait(&any_child);
		//printf("One of my son has finished.\nI'll start another one if I need to...");
		// one son has terminated
		current_checker_running--;
	} // end while
	return 0;
}
