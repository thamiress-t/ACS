#ifndef Attenuator_H
#define Attenuator_H

#include <string>
#include "Serial.h"

//#define DEBUG_ATTENUATOR

class Attenuator
{
private:
	Serial serial;
	std::string serial_number;
	const char* serial_port;
public:

	Attenuator& init(std::string serial_number);
	Attenuator();
	void AttenuatePot(double potency);
	~Attenuator();
};

std::string GetSN(const char* port_path);


#endif
