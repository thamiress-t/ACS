#include "Attenuator.h"

char predefined_port_paths[4][20] = { "/dev/ttyACM0","/dev/ttyACM1","path1", "path2"};

Attenuator::Attenuator() {

}

Attenuator& Attenuator::init(std::string serial_number)
{
	using namespace std;
	string SN;
	//four indicates the row_size of matrix "predefined_port_paths"
	for (int i = 0; i < 1; i++)
	{		
		printf("\nTrying ports...\n");
		SN=GetSN(predefined_port_paths[i]);
    	cout << ">>>> SN: " <<  SN << endl << flush;

		cout << endl;
		cout << "serial_number:" << serial_number << endl;
		cout << (SN.find(serial_number)!=std::string::npos) << " " ;
		cout << (int) (SN.find(serial_number)) << endl << flush;
		
	}
	
	this->serial_number = serial_number;
	
	return *this;
}


void Attenuator::AttenuatePot (double potency)
{
	char command[] = "SAA ";
	this->serial.SerialBegin(this->serial_port, B115200);
	this->serial.Write(strcat(command, std::to_string(potency).c_str()));
	this->serial.Close();
}


std::string GetSN(const char* port_path){
	using namespace std;
	char* serial_number;
	
	Serial serial;
	serial.SerialBegin(port_path, B115200);
	
	const char msg[] = "INFO";
	
	serial.Write(msg);	
	usleep(5000);
	
	stringstream ss;
	int pos;
	bool SN_search_ok=false;
	
	for(int i=0;i<10;++i) {
		serial.Read();
		ss << serial.read_buf;
		pos=ss.str().find("SN:");
		
		if(pos!=std::string::npos) 
		{	
			if((pos+12)<ss.str().size())
			{
				SN_search_ok=true;
				break;
			}
		}
	}
	
	
	string SN="";
	
	if(SN_search_ok) {
		SN=ss.str().substr(pos+3,12);
	}
	
	#ifdef DEBUG_ATTENUATOR
	cout << "********************************************" << endl;
	cout << "Buffer:" << endl << ss.str() << endl;
	cout << "********************************************" << endl;
	cout << "SN: " << SN << endl;
	cout << "********************************************" << endl;

	#endif
	serial.Close();
	
	return SN;
}

Attenuator::~Attenuator(){}
