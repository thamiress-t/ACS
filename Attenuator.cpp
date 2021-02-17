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

short int Attenuator::ConfigurationSetup(int serial_port){
	
	// Create new termios struc, we call it 'tty' for convention
	struct termios tty;
	
	// Read in existing settings, and handle any error
	if(tcgetattr(serial_port, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		return Attenuator::LogLevel::EXIT_ERROR;
	}
	
	
	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	
	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
	// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)
	
	tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;
	
	// Set in/out baud rate to be 115200
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);
	
	// Save tty settings, also checking for error
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return Attenuator::LogLevel::EXIT_ERROR;
	}
	
	return Attenuator::LogLevel::EXIT_SUCCESSFULLY;
}

char* Attenuator::GetSN(const char* port_path){
	char* serial_number;
	
	// Open the serial port
	int serial_port = open(port_path, O_RDWR);
	
	int handle = Attenuator::ConfigurationSetup(serial_port);
	
	unsigned const char msg[] = "INFO";
	Attenuator::Write(serial_port, msg,sizeof(msg));
	
	unsigned short int read_buf_size = 1024;
	char read_buf[read_buf_size];
	
	int feedback_size = Attenuator::Read(serial_port, read_buf, read_buf_size);
	
	//Search for the Serial Number
	for(int i = 0; i < feedback_size; i++){
		if(read_buf[i] == 'S'){
			if(read_buf[i+1] == 'N'){
				
				//Getting the SN element by element
				int j = i+4, SN_indx = 0;
				
						
				while(read_buf[j] != '\''){
					
					serial_number[SN_indx] = read_buf[j];
					
					SN_indx++;
					j++;
				}
				
				break;
			}
		}		
	}
	
	Attenuator::Close(serial_port);
	
	return serial_number;
}

void Attenuator::Write(short int serial_port, unsigned const char msg[], unsigned short int msg_size){
	// Write to serial port
	printf("\nWriting '%s'...\n", msg);
	write(serial_port, msg, msg_size);
}

short int Attenuator::Read(short int serial_port, char read_buf[], short int read_buf_size){
	
	char read_buf_aux[read_buf_size];
	// Read bytes. The behaviour of read() (e.g. does it block?,
	// how long does it block for?) depends on the configuration
	// settings above, specifically VMIN and VTIME
	printf("\nReading message...\n");
	int size = read(serial_port, &read_buf_aux, sizeof(read_buf_aux));
	
	read_buf = read_buf_aux;
	printf("---The read message was--- \n %s", read_buf);
	return 0;
}

void Attenuator::Close(short int serial_port){
	close(serial_port);
}

short int Attenuator::DefineSN(const char* port_path){
	
	char* serial_number = Attenuator::GetSN(port_path);
	
	if(serial_number == "Coloque o SN do atenuador 1"){
		return Attenuator::LogLevel::ATTENUATOR_1;
	}else if(serial_number == "Coloque o SN do atenuador 2"){
		return Attenuator::LogLevel::ATTENUATOR_2;
	}else if(serial_number == "Coloque o SN do atenuador 3"){
		return Attenuator::LogLevel::ATTENUATOR_3;
	}else if(serial_number == "Coloque o SN do atenuador 4"){
		return Attenuator::LogLevel::ATTENUATOR_4;
	}else{
		std::cout << "[ERROR]: Not a Serial Number" << std::endl;
		return Attenuator::LogLevel::EXIT_ERROR;
	}
	
	return Attenuator::LogLevel::EXIT_SUCCESSFULLY;
}


int main() {
	
	Attenuator attenuator1, attenuator2, attenuator3, attenuator4;
	unsigned short int attenuator[4];
	vector<std::string> port;
	
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
	
	double attenuation[2*4];
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
	
	//Writing to attenuators
	attenuator1.Write(attenuator1.serial_port ,strcat("SAA ", str(attenuation[0])), sizeof(strcat("SAA ", str(attenuation[0]))))
	attenuator2.Write(attenuator2.serial_port ,strcat("SAA ", str(attenuation[1])), sizeof(strcat("SAA ", str(attenuation[1]))))
	attenuator3.Write(attenuator3.serial_port ,strcat("SAA ", str(attenuation[2])), sizeof(strcat("SAA ", str(attenuation[2]))))
	attenuator4.Write(attenuator4.serial_port ,strcat("SAA ", str(attenuation[3])), sizeof(strcat("SAA ", str(attenuation[3]))))
	
	
	return 0; 
}
