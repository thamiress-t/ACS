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

	Attenuator(char const* serial_number);
	void AttenuatePot(double potency);
	~Attenuator();
};

char* GetSN(const char* port_path);


#endif