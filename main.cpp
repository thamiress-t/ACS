#include  <string>
#include "Attenuator.h"


int main() {
	
	using namespace std;
	Attenuator  attenuator3;//, attenuator4("R3160950387");
	
	string SN="R3160950387";
	string test=" Teste R316 fim\n";
	
	//cout << SN << endl;
	//cout << test << endl;
	//cout << (int)test.find(SN) << endl << flush;

	usleep(500000);
	
	
	attenuator3.init(SN);
	
	
/*	
	std::ifstream file("tabela_teste.txt");
	
	double attenuation[2*4];
	std::string line;
	int n_lines = 0, i= 0;
	std::cout << "\nReading file..." << std::endl;
	while(std::getline(file, line)){
			std::istringstream is(line);
			while(is >> attenuation[i]){
				i++;
			}
			n_lines++;
	}
	
	printf("The file is: \n");
	
	for(int i = 0; i < n_lines*4; i++){
		printf(" %f",attenuation[i]);
	}
	
	attenuator3.AttenuatePot(attenuation[1]);
	attenuator4.AttenuatePot(attenuation[2]);
	*
	* 
	* 
	* 
	* 
	*  
	*/
	
	return 0; 
}
