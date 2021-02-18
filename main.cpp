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


int main() {
	
	Attenuator attenuator1, attenuator2, attenuator3, attenuator4;
	unsigned short int attenuator[4];
	std::vector<std::string> port;
	
	port.push_back("Caminho da porta 1");
	port.push_back("Caminho da porta 2");
	port.push_back("Caminho da porta 3");
	port.push_back("Caminho da porta 4");
	
	attenuator[0] = Attenuator::DefineSN(port[0]);
	attenuator[1] = Attenuator::DefineSN(port[2]);
	attenuator[2] = Attenuator::DefineSN(port[3]);
	attenuator[3] = Attenuator::DefineSN(port[4]);
	
	//Define Attenuators
	for(int i = 0; i < 4; i++){
		switch(attenuator[i]){
			case Attenuator::LogLevel::ATTENUATOR_1:
				attenuator1.serial_number = "Coloque Serial Number do atuador 1";
				attenuator1.serial_port = port[i]; 
			break;
			case Attenuator::LogLevel::ATTENUATOR_2:
				attenuator2.serial_number = "Coloque Serial Number do atuador 2";
				attenuator2.serial_port = port[i]; 
			break;
			case Attenuator::LogLevel::ATTENUATOR_3:
				attenuator3.serial_number = "Coloque Serial Number do atuador 3";
				attenuator3.serial_port = port[i]; 
			break;
			case Attenuator::LogLevel::ATTENUATOR_4:
				attenuator4.serial_number = "Coloque Serial Number do atuador 4";
				attenuator4.serial_port = port[i]; 
			break;
			default:
				std::cout << "[ERROR]: Bad definition for attuators" << std::endl;
				return 1;
		}
	}
	
	
	
	std::ifstream file("tabela_teste.txt");
	
	double attenuation[2*4];//attenuation_value
	std::string line;
	int n_lines = 0, i= 0;
	std::cout << "\nReading file..." << std::endl;
	while(std::getline(file, line)){
			std::istringstream is(line);
			while(is >> attenuation[i]){
				i++;
			}
			n_lines++;
	}
	
	printf("The file is: \n");
	
	for(int i = 0; i < n_lines*4; i++){
		printf(" %f",attenuation[i]);
	}
	
	//Writing values to serial connected devices
	attenuator1.Write(attenuator1.serial_port ,strcat("SAA ", std::str(attenuation[0])), sizeof(strcat("SAA ", str(attenuation[0]))))//this.serial_port
	attenuator2.Write(attenuator2.serial_port ,strcat("SAA ", std::str(attenuation[1])), sizeof(strcat("SAA ", str(attenuation[1]))))
	attenuator3.Write(attenuator3.serial_port ,strcat("SAA ", std::str(attenuation[2])), sizeof(strcat("SAA ", str(attenuation[2]))))
	attenuator4.Write(attenuator4.serial_port ,strcat("SAA ", std::str(attenuation[3])), sizeof(strcat("SAA ", str(attenuation[3]))))
	
	
	return 0; 
}
