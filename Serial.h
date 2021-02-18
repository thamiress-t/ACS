#ifndef Serial_H
#define Serial_H

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



struct BufferOutput{
	char* buffer;
	short unsigned int buffer_size;
};

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
	short unsigned int buffer_size;
public:
	void SerialBegin (const char* port_path, unsigned short int baudrate);
	void Write(const char* msg);
	void Read();
	void Close();
	BufferOutput GetBuffer();
	
	


};
#endif