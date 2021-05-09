/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TCAT.cpp
 * Author: daniel
 * 
 * Created on 17 de Março de 2021, 15:30
 */

#include "TAcs.h"

TAcs::TAcs() {


#ifdef ARCH_RASP
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialisation failed\n");
        exit(1);
    }

    /* Set GPIO modes */
    gpioSetMode(4, PI_OUTPUT);
    gpioSetMode(17, PI_OUTPUT);
    gpioSetMode(27, PI_OUTPUT);
    gpioSetMode(22, PI_INPUT);
    gpioSetMode(10, PI_OUTPUT);
    gpioSetMode(9, PI_OUTPUT);
    gpioSetMode(11, PI_OUTPUT);
    gpioSetMode(0, PI_OUTPUT);

    gpioSetMode(5, PI_OUTPUT);
    gpioSetMode(6, PI_OUTPUT);
    gpioSetMode(13, PI_OUTPUT);
    gpioSetMode(19, PI_INPUT);
    gpioSetMode(26, PI_OUTPUT);
    gpioSetMode(21, PI_OUTPUT);
    gpioSetMode(20, PI_OUTPUT);
    gpioSetMode(16, PI_OUTPUT);

    gpioSetMode(12, PI_OUTPUT);
    gpioSetMode(1, PI_OUTPUT);
    gpioSetMode(7, PI_OUTPUT);
    gpioSetMode(8, PI_INPUT);
    gpioSetMode(25, PI_OUTPUT);
    gpioSetMode(24, PI_OUTPUT);
    gpioSetMode(23, PI_OUTPUT);
    gpioSetMode(18, PI_OUTPUT);
#endif

}

void TAcs::setPhases() {
	float* ph_test = getPhases();
	if(this->isInt())
		for(int i = 0; i < 3; i++)
			this->ps[i].setPhaseDeg(ph_test[i]);
	else
		for(int i = 0; i < 3; i++)
			this->ps[i].setPhaseInt(ph_test[i]);
}

void TAcs::connectAttenuators(){
	this->att0.connect();
	if(this->att0.isConnected()) 
        std::cout << "conexão ok! " << att0.getSerialPort() << std::endl;
	
	this->att1.connect();
	if(this->att1.isConnected()) 
        std::cout << "conexão ok! " << att1.getSerialPort() << std::endl;
	
	this->att2.connect();
	if(this->att2.isConnected()) 
        std::cout << "conexão ok! " << att2.getSerialPort() << std::endl;
	
	this->att3.connect();
	if(this->att3.isConnected()) 
        std::cout << "conexão ok! " << att3.getSerialPort() << std::endl;
}

void TAcs::AttenuatePot(){
	float* pot = this->getAttenuations();
	/* for(int i = 0; i < 4; i++)
		printf("%f ", pot[i]); */
	
	this->att0.AttenuatePot(pot[0]);
	this->att1.AttenuatePot(pot[1]);
	this->att2.AttenuatePot(pot[2]);
	this->att3.AttenuatePot(pot[3]);
}

void TAcs::unconnectAttenuators(){
	this->att0.unconnect();
	this->att1.unconnect();
	this->att2.unconnect();
	this->att3.unconnect();
}


TAcs::TAcs(const TAcs& orig) {
	
}


TAcs::~TAcs() {
#ifdef ARCH_RASP
    /* Stop DMA, release resources */
    gpioTerminate();
#endif
}