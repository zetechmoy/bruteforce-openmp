// multiattack.c
// Starts multiple instances checking if a password can be found in the dictionnary.
// usage : nb_of_processus dictionnary_file shasum_file


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <omp.h>
#include <errno.h>
#include <string.h>

//read next line of a file
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

void writeline(char * filename, const char * content){
	FILE * fp;
	fp = fopen (filename, "a");
	fprintf(fp, "%s", content);
	fprintf(fp, "\n");
	fclose (fp);
}


int main(int argc, char *argv[]) {
	if(argc < 2) fprintf(stderr, "Usage: '%s' nb_of_processus nb_of_checker dictionnary_file shasum_file\n", argv[0]), exit(EXIT_FAILURE);

	char *p;
	//We have MAX_PROC which can run MAX_CHECKER checker proc on each processor
	long MAX_PROC = strtol(argv[1], &p, 10);
	long MAX_CHECKER = strtol(argv[2], &p, 10);
	char * dict_file = argv[3];
	char * shasum_file = argv[4];

	printf("MAX_PROC = %ld\n", MAX_PROC);
	printf("MAX_CHECKER = %ld\n", MAX_CHECKER);
	printf("dict_file = %s\n", dict_file);
	printf("shasum_file = %s\n", shasum_file);

	// opening file
	FILE * ds = fopen(shasum_file, "r");
	if (ds == NULL) exit(EXIT_FAILURE);

	//vars for each proc
	int current_checker_running = 0;

	//each processor is gonna execute that code
	#pragma omp parallel num_threads(MAX_PROC) shared(current_checker_running, ds)
	{
		char * current_password_to_analyse = "start";

		//while we have aa word to crack
		while(current_password_to_analyse != NULL){

			//if we can run a checker
			if(current_checker_running < MAX_CHECKER){
				int child_pid;
				//so we fork current processor which run the checker
				if((child_pid=fork()) < 0){
					//if we can't run the checker so we wait for another checker to finish
					pid_t any_child;
					wait(&any_child);
					current_checker_running--;

				}else if (child_pid != 0) {//processor
					//take the next word to crack
					//must be synchronised
					#pragma omp critical
					{
						current_checker_running++;
						current_password_to_analyse = readline(ds);
					}

				} else {//fork
					//printf("current_password_to_analyse : %s\n", current_password_to_analyse);
					execl("/bin/grep", "grep", "--binary-files=text", current_password_to_analyse, dict_file, (char *)NULL);
				}

			}else{
				//if we have too much checker so we wait for one of them to finish
				pid_t any_child;
				wait(&any_child);
				current_checker_running--;
			}

		}
	}

	return 0;
}
