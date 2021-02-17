#ifndef Attenuator_H
#define Attenuator_H

class Attenuator{
	public:
	
	enum LogLevel{
		EXIT_ERROR,
		EXIT_SUCCESSFULLY,
		ATTENUATOR_1,
		ATTENUATOR_2,
		ATTENUATOR_3,
		ATTENUATOR_4
	};
	
	char* GetSN(const char* port_path);
	short int ConfigurationSetup(int serial_port);
	short int DefineSN(const char* port_path);
	void Write(short int serial_port, unsigned const char msg[], unsigned short int msg_size);
	short int Read(short int serial_port, char read_buf[], short int read_buf_size);
	void Close(short int serial_port);
	void DefineSN();
	
	private:
	unsigned short int serial_number;
	unsigned short int serial_port;
};


#endif