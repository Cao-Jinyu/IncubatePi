#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
     
 
#define BUF_READ_SIZE 15
#define PERIOD 40000		// PWM period in nanoseconds
#define MIN_PERIOD 10000	// Minimum allowed PWM period	
 
/* 
	Checks that the PWM module is loaded in the linux kernel.
	This is done by searching the linux module list and confirming that 
	there is at least one module containing the string 'pwm'.
	Returns 1 on failure.
	Returns 0 if module is loaded correctly.
*/
int PWMModuleCheck(){
	
	// Linux command that writes any modules containing the string pwm to a file temp
	char moduleCheck[] = "lsmod | grep pwm > temp";		
	
	// Executes the above linux command
	check = system(moduleCheck);
	if (check == -1){
		printf("Couldn't execute lsmod linux system command\n") 
		return 1;
	}
	
	// Opens the file where the output of the command was piped to
	fd = open("temp",O_RDONLY);
	if(fd == -1) {
		printf("Couldn't open temp file\n");
		return 1; 
	}	
	
	// Checks that the file is not empty and prints out the module name 
	if (!read(fd, buf, BUF_READ_SIZE)){
		printf("Couldn't find the neccessary kernal modules\n");
		close(fd);
  		return 1;
	} else printf("Module: %s\n", buf);	 	
	close(fd);
	return 0;
	
}

/*

*/
int setPeriod(int period){
	
	int fd, digits;
	char periodFile[] = "/sys/class/pwm/pwmchip0/pwm1/period";
	
	// Check that the period is valid
	if (period <= MIN_PERIOD){
		printf("Invalid period\n");
		return 1;
	}
	
	// Open the linux PWM period file
	fd = open(periodFile, O_WRONLY | O_TRUNC);
	if(fd == -1) {
		printf("Couldn't open period file\n");
		return 1;   
	}

	// Write out period 
	digits = floor(log10(period)) + 1;
	write(fd,period,digits);
	currentPeriod = period;
	close(fd);
	return 0;
}

/*

*/ 
int setDutyCycle(int dutyCycle){
	
	int fd, digits;
	char dutyCycleFile[] = "/sys/class/pwm/pwmchip0/pwm1/duty_cycle";
	
	// Check that the duty cycle is valid
	if (dutyCyle <= 0){
		printf("Invalid duty cycle\n")
		return 1;
	}
	
	
	fd = open(dutyCycleFile, O_WRONLY | O_TRUNC);
	if(fd == -1) {
		printf("Couldn't open duty cycle file\n");
		return 1;   
	}
	write(fd,"0\n",2);
	close(fd);


int main (void) {

	/* 
		uname -r       					: Check kernel version
		/boot/config.txt				: Edit this file
		dtoverlay=pwm,pin=13,func=4		: Add this line		
	*/

 	int fd;
 	char buf[BUF_READ_SIZE];   
 	char exportFile[] = "/sys/class/pwm/pwmchip0/export";
 	
 	char dutyCycleFile[] = "/sys/class/pwm/pwmchip0/pwm1/duty_cycle";
 	char enableFile[] = "/sys/class/pwm/pwmchip0/pwm1/enable";
	
	if (PWMModuleCheck()) return 1;
	
	// Set up the pwm1 directory
	fd = open(exportFile, O_WRONLY | O_TRUNC);
	if(fd == -1) {
		printf("Couldn't open export file\n");
		return 1;   
	}
	write(fd,"1\n",2);
	close(fd);
	
	// Set up period
	

	
	// Set up duty cycle
	fd = open(dutyCycleFile, O_WRONLY | O_TRUNC);
	if(fd == -1) {
		printf("Couldn't open duty cycle file\n");
		return 1;   
	}
	write(fd,"0\n",2);
	close(fd);
	
	// Set up enable
	fd = open(enableFile, O_WRONLY | O_TRUNC);
	if(fd == -1) {
		printf("Couldn't open enable file\n");
		return 1;   
	}
	write(fd,"1\n",2);
	close(fd);
 	
 	char dutyCycle[10];
 	int i;
 	//for (;;) {
 	 	for (i = 0; i <= 40000; i+=10000){
 	 		fd = open(dutyCycleFile, O_WRONLY | O_TRUNC);
			if(fd == -1) {
				printf("Couldn't open duty cycle file\n");
				return 1;   
			}
			sprintf(dutyCycle, "%d\n", i);
			write(fd,dutyCycle,strlen(dutyCycle));
			close(fd);
			sleep(5);
		}
	// }
 	 		
}

