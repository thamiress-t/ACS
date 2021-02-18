#ifndef Attenuator_H
#define Attenuator_H

#include "Serial.h"


class Attenuator
{
private:
	Serial serial;
	const char* serial_number;
	const char* serial_port;
public:

	Attenuator (const char* serial_number);
	int Atenuar(float pot);
	const char* GetSN(const char* port_path);
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

#endif