#include "Attenuator.h"


int main() {
	Attenuator attenuator1("SN1"), attenuator2("SN2"), attenuator3("SN3"), attenuator4("SN4");
	
	
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
	
	return 0; 
}
