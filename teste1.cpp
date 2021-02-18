/*Classe Serial

Requisitos funcionais:
Metodo para configurar a porta serial
Metodo para ler
Metodo para escrever


Classe atenuador
Serial
Requisitos funcionais:
Metodo para verificar a o nÃºmero do serial do atenuador
MÃ©todo para atenuar
*/
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
#include <sstream>
//#include <iostream>
//#include <typeinfo>


char predefined_port_paths[4][10] = { "Joao", "Maria", "Jose" };

enum LogLevel
{
	EXIT_ERROR = 0,
	EXIT_SUCCESSFULLY,
	ATTENUATOR_1,
	ATTENUATOR_2,
	ATTENUATOR_3,
	ATTENUATOR_4
};

class Serial
{
private:
	int serial_port;
	struct termios tty;
	char read_buf[256];

public:
	void SerialBegin (const char* port_path, unsigned short int baudrate);
	void Write(const char* msg);
	void Read();
	void Close();
	char* GetBuffer(){
		return this->read_buf;
	}


};

void Serial::SerialBegin(const char* port_path, unsigned short int baudrate = B115200)
{
	this->serial_port = open(port_path, O_RDWR);

	// Create new termios struc, we call it 'tty' for convention
	this->tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	this->tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	this->tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
	this->tty.c_cflag |= CS8; // 8 bits per byte (most common)
	this->tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	this->tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	this->tty.c_lflag &= ~ICANON;
	this->tty.c_lflag &= ~ECHO; // Disable echo
	this->tty.c_lflag &= ~ECHOE; // Disable erasure
	this->tty.c_lflag &= ~ECHONL; // Disable new-line echo
	this->tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	this->tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	this->tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes
    
	this->tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	this->tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
	// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

	this->tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	this->tty.c_cc[VMIN] = 0;
    
	// Set in/out baud rate to be 115200
	cfsetispeed(&tty, baudrate);
	cfsetospeed(&tty, baudrate);

	// Save tty settings, also checking for error
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
	{
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}

	// Read in existing settings, and handle any error
	if(tcgetattr(serial_port, &tty) != 0)
	{
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}
}



void Serial::Write(const char* msg)
{
	short unsigned int msg_size = (strlen(msg) + 1) * sizeof(char);
	printf("\nWriting '%s'...\n", msg);
	write(this->serial_port, msg, msg_size);
}

void Serial::Read()
{
	// Normally you wouldn't do this memset() call, but since we will just receive
	// ASCII data for this example, we'll set everything to 0 so we can
	// call printf() easily.
	memset(&this->read_buf, '\0', sizeof(this->read_buf));
	
	// Read bytes. The behaviour of read() (e.g. does it block?,
	// how long does it block for?) depends on the configuration
	// settings above, specifically VMIN and VTIME
	int num_bytes = read(this->serial_port, &this->read_buf, sizeof(this->read_buf));
}

	   void Serial::Close()
{

	close(this->serial_port);
}

class Attenuator
{
private:
	Serial serial;
	const char* serial_number;
	const char* serial_port;
public:

	Attenuator (const char* serial_number);
	void AttenuatePot(double potency);
};

Attenuator::Attenuator (const char* serial_number)
{
	int defined_port;
	//four indicates the row_size of matrix "predefined_port_paths"
	for (int i = 0; i < 4; i++)
	{
		this->serial.SerialBegin(predefined_port_paths[i]);

		const char msg[] = "INFO";
		this->serial.Write(msg);
		
		if (strcmp(serial_number,this->serial.GetBuffer())==0)
		{
			break;
		}	
     	 this->serial.Close();
	}
	
	this->serial_port = predefined_port_paths[defined_port];
	this->serial_number = serial_number;
}

void Attenuator::AttenuatePot (double potency)
{
	char command[] = "SAA ";
	this->serial.SerialBegin(this->serial_port);
	this->serial.Write(strcat(command, std::to_string(potency).c_str()));
	this->serial.Close();
}
