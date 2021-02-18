#include "Attenuator.h"

char predefined_port_paths[4][10] = { "Joao", "Maria", "Jose" };

Attenuator::Attenuator(char const* serial_number)
{
	
	//four indicates the row_size of matrix "predefined_port_paths"
	for (int i = 0; i < 4; i++)
	{		
		if (strcmp(serial_number,GetSN(predefined_port_paths[i]))==0)
		{
			break;
		}
	}
	
	this->serial_number = serial_number;
}


void Attenuator::AttenuatePot (double potency)
{
	char command[] = "SAA ";
	this->serial.SerialBegin(this->serial_port, B115200);
	this->serial.Write(strcat(command, std::to_string(potency).c_str()));
	this->serial.Close();
}


char* GetSN(const char* port_path){
	char* serial_number;
	
	Serial serial;
	serial.SerialBegin(port_path, B115200);
	
	const char msg[] = "INFO";
	serial.Write(msg);
	
	
	
	
	serial.Read();
	
	BufferOutput read_buf = serial.GetBuffer();
	
	//Search for the Serial Number
	for(int i = 0; i < read_buf.buffer_size; i++){
		if(read_buf.buffer[i] == 'S'){
			if(read_buf.buffer[i+1] == 'N'){
				
				//Getting the SN element by element
				int j = i+4, SN_indx = 0;
				
						
				while(read_buf.buffer[j] != '\''){
					
					serial_number[SN_indx] = read_buf.buffer[j];
					
					SN_indx++;
					j++;
				}
				
				break;
			}
		}		
	}
	
	serial.Close();
	
	return serial_number;
}

Attenuator::~Attenuator(){}