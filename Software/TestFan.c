#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
     
#define BUF_READ_SIZE 15
#define PERIOD "40000\n"      // PWM period in nanoseconds
   
int main (void) {

	/* 
		uname -r       					: Check kernel version
		/boot/config.txt				: Edit this file
		dtoverlay=pwm,pin=13,func=4		: Add this line		
	*/
	
 	char moduleCheck[] = "lsmod | grep fjes > temp";
 	int check; 
 	int fd;
 	char buf[BUF_READ_SIZE];   
 	//char exportFile[] = "/sys/class/pwm/pwmchip0/export";
 	//char periodFile[] = "/sys/class/pwm/pwmchip0/pwm1/period";
 	//char dutyCycleFile[] = "/sys/class/pwm/pwmchip0/pwm1/duty_cycle";
 	//char enableFile[] = "/sys/class/pwm/pwmchip0/pwm1/enable";
    char exportFile[] = "temp2";
    char periodFile[] = "temp3";
    char dutyCycleFile[] = "temp4";
    char enableFile[] = "temp5";
	
	// Check that the necessary kernel module is availible 
	check = system(moduleCheck);		
	fd = open("temp",O_RDONLY);
	if(fd == -1) {
		printf("Couldn't open temp file\n");
		return 1;   
	}
	if (!read(fd, buf, BUF_READ_SIZE)) check = -1;	
	else printf("Module: %s\n", buf);	 	
	close(fd);
	if (check == -1){
		printf("Couldn't find the neccessary kernal modules\n");
  		return 1;
	} 
	
	// Set up the pwm1 directory
	fd = open(exportFile, O_WRONLY | O_TRUNC);
	if(fd == -1) {
		printf("Couldn't open export file\n");
		return 1;   
	}
	write(fd,"1\n",2);
	close(fd);
	
	// Set up period
	fd = open(periodFile, O_WRONLY | O_TRUNC);
	if(fd == -1) {
		printf("Couldn't open period file\n");
		return 1;   
	}
	write(fd,"40000\n",6);
	close(fd);
	
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

