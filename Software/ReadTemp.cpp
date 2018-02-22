#include <iostream>
#include <dirent.h>
#include <string>

using namespace std; 

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define W1GPIO 		modprobe w1-gpio
#define W1THERM 	modprobe w1-therm
#define W1PATH 		/sys/bus/w1/devices/

/* 
	This class is used to read the temperature from a DS18B20 using the RPI's one wire interface
*/
class TempReader{

	private:
		char * w1file[128];
	
	public:
		static void loadKernelModules();
		void getFileName();
		float readTemp();
		
};
   
/*
	Adds the neccessary loadable kernel modules.
	This must be run prior to executing any other functions in this class 
	Returns 0 on success
	Returns 1 on failure
*/    
static void TempReader::loadKernelModules(){
	if (system(TOSTRING(W1GPIO)) == -1 || system(TOSTRING(W1THERM)) == -1){
		cerr << "Couldn't load the neccessary kernal modules" << endl;
  		return 1;
	}
	return 0;
}     
    
/* 
	Obtains the name of the file that w1 linux system file.
	This file has a variable name.
*/
void TempReader::getFileName(){   

	DIR *dir;
 	struct dirent *dirent;
  
 	dir = opendir(W1PATH);
 	if (dir){
  		while ((dirent = readdir(dir))) 
  			if (strstr(dirent->d_name, "28-")) 
  				sprintf(w1file, "%s/%s/w1_slave", W1PATH, dirent->d_name);
		closedir(dir);
	} else {
  		cerr << "Couldn't open the w1 devices directory" << endl;
  		return 0;
 	} 
 	
}

float TempReader::readTemp(){
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
int main (void) {

 	char buf[256];     // Data from device
 	char tmpData[6];   // Temp C * 1000 reported by device 
 	

 	
 	ssize_t numRead;
 

	

}

