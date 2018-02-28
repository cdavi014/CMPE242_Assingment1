/**
 * Author: Carlos R. Davila
 * SJSU ID: 010779067
 * Last Modified: Feb-23-2-18
 * 
 * Notes:
 * Program assumes that the module has been loaded already.
 */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

static FILE *fd;

void exit_handler(int signo) {
	if(signo == SIGINT) {
		printf("\n\n[SIGINT] Cleaning up...\n");
		if(fd != NULL) fclose(fd);
		exit(1);
	}
}

int main() {
	signal(SIGINT, exit_handler);
	
	int input;
	fd = fopen("/sys/kernel/led_ctrl_pwr/led_pwr", "w");
	
	if(fd == NULL) {
		printf("Error opening file!\n");
	}
	
	while(1) {
		printf("Enter 0 [LED OFF], or 1 [LED ON]:");
		scanf("%d", &input);
		if(input == 0){
			fprintf(fd, "0");
		} else if(input == 1) {
			fprintf(fd, "1");
		} else {
			printf("[ERROR] Input must be \'0\' or \'1\'\n");
		}
		fflush(fd);
	}
	
	return 0;
}
