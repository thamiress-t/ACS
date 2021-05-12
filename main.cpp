#include  <string>
#include "Attenuator.h"
//#define NDEBUG
#include <assert.h>
#include "ToolBoxAndUtilities.h"
#include "SystemTests.h"
#include <fstream>
#include <vector>
#include <map>
#include "TTable.h"
#include "TCommand.h"
#include "TAcs.h"
#include "Attenuator.h"
#include <pigpio.h>


int main(int argc, char** argv) {

    using namespace std;

	TAcs teste;
	//Attenuator att("R3160950386");
	teste.parseCmd(argc,argv);
	teste.connectAttenuators();
	teste.AttenuatePot();
	
	//att.connect();
	//att.AttenuatePot(50);
	teste.setPhases();
	//teste.printTest();
	
	
	
	//teste.printTest();
	//teste.parseCmd("cmd -pi 1 2 3 4 5 6 7");
    //testTCommand();
    //testTTablet();
    //testToolBoxAndUtilities();
    //testTSerial();
    //testGetSN();
    //testAttenuator();
	
	
    return 0;
}

    