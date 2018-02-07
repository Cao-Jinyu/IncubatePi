#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
     
int main (void) {

	DIR *dir;
 	struct dirent *dirent;
 	
 	char w1file[128]; // Path to device
 	char buf[256];     // Data from device
 	char tmpData[6];   // Temp C * 1000 reported by device 
 	
 	char path[] = "/sys/bus/w1/devices/";
 	char w1gpio[] = "modprobe w1-gpio";
 	char w1them[] = "modprobe w1-therm";
 	
 	ssize_t numRead;
 
	// Load the necessary kernel modules 
	if (system(w1gpio) == -1 || system(w1them) == -1){
		perror ("Couldn't load the neccessary kernal modules");
  		return 1;
	}	

	// Open the 1-wire directory
 	dir = opendir(path);
 	if (dir != NULL){
  		while ((dirent = readdir(dir))) 
  			if (strstr(dirent->d_name, "28-")) 
  				sprintf(w1file, "%s/%s/w1_slave", path, dirent->d_name);
		closedir(dir);
	} else {
  		perror ("Couldn't open the w1 devices directory");
  		return 1;
 	}

    // Assemble path to 1-Wire device
 	while(1) {
 		int fd = open(w1file, O_RDONLY);
  		if(fd == -1) {
   			perror ("Couldn't open the w1 device.");
   			return 1;   
  		}
  		while((numRead = read(fd, buf, 256)) > 0){
   			strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
   			float tempC = strtof(tmpData, NULL);
   			printf("Temp: %.3f C\n", tempC / 1000);
  		}
  		close(fd);
 	}	 
}

