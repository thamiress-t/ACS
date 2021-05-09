/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TCommand.cpp
 * Author: daniel
 * 
 * Created on 17 de Março de 2021, 17:22
 */

#include <cstring>

#include "TCommand.h"
#include "TTable.h"

TCommand::TCommand() {
}

TCommand::TCommand(const TCommand& orig) {
}

TCommand::~TCommand() {
}

TCommand& TCommand::parseCmd(const char *cmd, const char *fs) {
    using namespace std;

    int N = strlen(cmd);
    char *str = (char*) malloc((N + 1) * sizeof (char));
    strcpy(str, cmd);
    char *argv[10];
    char *ptr = 0;
    ptr = strtok(str, fs);
    argv[0] = ptr;
    int k = 1;

    while (ptr != NULL) {
        //printf("'%s'\n", ptr);
        ptr = strtok(NULL, fs);
        argv[k] = ptr;
        k++;
        if (k > 9)
            break;
    }

    this->parseCmd(k-1, argv);

    free(str);
    return *this;
}

/*
 * Modo comando de linha:
 * acs set -pd <ph1> <ph2> <ph3> [ <at0> <at1> <at2> <at3> ]
 * acs set -pi <ph1> <ph2> <ph3> [ <at0> <at1> <at2> <at3> ]
 * acs -idx <nidx>
 * acs set -pd <ang> <arrangement>
 * acs set -pi <ang> <arrangement>
 * acs -help
 */
TCommand& TCommand::parseCmd(int argc, char** argv) {
    using namespace std;
	//printf("%d \n\n", argc);
    switch(argc){
		case 1:
			MSGE << "Not enough arguments!" << endl;
			break;
		case 2:
			if (strcmp(argv[1], "-help") == 0) {
				cout << "Usage:" << endl;
				cout << "acs set -pd <ph1> <ph2> <ph3> [ -at <at0> <at1> <at2> <at3> ]" << endl;
				cout << "acs set -pi <ph1> <ph2> <ph3> [ -at <at0> <at1> <at2> <at3> ]" << endl;
				cout << "acs -idx <nidx>" << endl;
				cout << "acs -pd <ang> <arrangement>" << endl;
				cout << "acs -help" << endl;
			}else{
				MSGE << "Invalid command!" << endl;
			}
			break;
		case 3:
			if (strcmp(argv[1], "-idx") == 0) {
			}else{
				MSGE << "Invalid command!" << endl;
			}
			break;
		case 5:
			if (strcmp(argv[2], "-pi") == 0) {
				inputType = 1;
				TTable tab;
				tab.load(strcat(argv[4],"_angulos20_60_135.csv"));//"arranjo1_angulos20_60_135.csv");
				short int angle = (short int)strtod(argv[3],NULL);
				
				TTable::TLVector v=tab.lineVector(angle);
				
				for(int i=0;i<v.size() - 1;++i)
					args[i] = v[i + 1];
				break;
			}
			if(strcmp(argv[2], "-pd") == 0){
				inputType = 0;
				TTable tab;
				tab.load(strcat(argv[4],"_angulos20_60_135.csv"));//"arranjo1_angulos20_60_135.csv");
				short int angle = (short int)strtod(argv[3],NULL);
				
				TTable::TLVector v=tab.lineVector(angle);
				
				for(int i=0;i<v.size() - 1;++i)
					args[i] = v[i + 1];
				break;
			}
			MSGE << "Invalid command!" << endl;
			break;
		case 10:
			if (strcmp(argv[2], "-pi") == 0) {
				for(int i = 0; i < 7; i++)
					this->args[i] = (float)strtod(argv[i + 3],NULL);
				break;
			}
			if(strcmp(argv[2], "-pd") == 0){
				for(int i = 0; i < 7; i++)
					this->args[i] = (float)strtod(argv[i + 3],NULL);
				break;
			}
			MSGE << "Invalid command!" << endl;
			break;
		default:
			MSGE << "Invalid command!" << endl;
		
	}
    return *this;
}

void TCommand::printTest() {
	for(int i = 0; i< 7; i++)
		std::cout << this->args[i] << std::endl;
}

float* TCommand::getAttenuations(){
	static float output[4];
	for(int i = 3; i< 7; i++)
		output[i - 3] = this->args[i];
	return output;
}

float* TCommand::getPhases(){
	static float output[3];
	for(int i = 0; i< 3; i++)
		output[i] = this->args[i];
	return output;
}


bool TCommand::isInt(){
	return this->inputType;
}