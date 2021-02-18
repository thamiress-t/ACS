#include "Attenuator.h"
#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <vector>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <fstream>
#include "Attenuator.h"
#include <sstream>
//#include <iostream>
//#include <typeinfo>


void funcao(char* msg){
	std::cout << "Este é o tamanho na função: "<< (strlen(msg)+1)*sizeof(char)<< std::endl;
}

int main() {
	 char msg[] = "o";
	std::cout << typeid(B115200).name() << std::endl;
	std::cout << "Este é o tamanho na main: "<< sizeof(msg)<< std::endl;
	
	funcao(msg);
	
	
	return 0; 
}
