#include "Serial.h"


void Serial::SerialBegin(const char* port_path, unsigned short int baudrate = B115200)
{
	printf("\nOpening port: '%s'\n", port_path);
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
	usleep(50000);
	tcflush(this->serial_port,TCIOFLUSH);
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
	memset(&this->read_buf,0, this->Nbuffer);
	
	// Read bytes. The behaviour of read() (e.g. does it block?,
	// how long does it block for?) depends on the configuration
	// settings above, specifically VMIN and VTIME
	short unsigned int num_bytes = read(this->serial_port, &this->read_buf,this->Nbuffer);

#ifdef DEBUG_SERIAL
		printf("INFO: %s N=%d\n",read_buf,num_bytes);
#endif

	this->buffer_size = num_bytes;

/*	printf("\nThe read information is: '%s'\n",this->read_buf);
	if (num_bytes < 0) {
      printf("Error reading: %s", strerror(errno));
    }*/

}

	   void Serial::Close()
{

	close(this->serial_port);
}


BufferOutput Serial::GetBuffer()
{
	BufferOutput out_buffer;
	
	out_buffer.buffer = this->read_buf;
	out_buffer.buffer_size = this->buffer_size;
	return out_buffer;
}
