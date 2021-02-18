#include "Attenuator.h"


Attenuator::Attenuator (const char* serial_number)
{
	
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
	
	this->serial_number = serial_number;
}


void Attenuator::AttenuatePot (double potency)
{
	char command[] = "SAA ";
	this->serial.SerialBegin(this->serial_port);
	this->serial.Write(strcat(command, std::to_string(potency).c_str()));
	this->serial.Close();
}
