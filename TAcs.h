/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TCAT.h
 * Author: daniel
 *
 * Created on 17 de Março de 2021, 15:30
 */

#ifndef TACS_H
#define TACS_H


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string.h>
#include "TPhShifter.h"
#include "TCommand.h"
#include "Attenuator.h"
/*
 * No Raspberry:
 Habilitar -> ARCH_RASP
 * defasador.cpp

   gcc -o defasador defasador.c -lpigpio -lrt -lpthread

   sudo ./defasador
 */


#define ARCH_RASP 1
#define DEBUG_MODE 1

#ifdef ARCH_RASP
#include <pigpio.h>
#endif

class TAcs: public TCommand{
private:
	TPhShifter ps[3];
	Attenuator att0{"R3160950384"},att1{"R3160950386"},att2{"R3160950383"},att3{"R3160950387"};
public:
	
    TAcs(const TAcs& orig);
    TAcs();
    virtual ~TAcs();
	
	
	void setPhases();
	
	//Attenuator
	void connectAttenuators();
	void AttenuatePot();
	void unconnectAttenuators();
	//Phase Shifter
    TAcs& loadParameters(std::string file = "offset.txt") {
        using namespace std;

        ifstream phOffset;

        phOffset.open(file, std::ifstream::in);

        if (!phOffset) {
            cerr << "Arquivo " << file << " não encontrado!!!";
            cout << "Utilizando fases padrões." << endl;
        } else {
#ifdef DEBUG_MODE
            cout << "Lendo arquivo de offsets." << endl;
#endif
            float x;
            for (int i = 0; i < 3; ++i) {
                phOffset >> x;
                ps[i].setOffset(x);

#ifdef DEBUG_MODE
                cout << "D(" << i << ")_off=" << ps[i].getOffset() << endl;
#endif
            }
        }

        phOffset.close();
        return *this;
    }

    TAcs& saveParameters(std::string file = "offset.txt") {
        using namespace std;
        ofstream phOffsetOut;
        cout << "Criando arquivo " << file << endl;
        phOffsetOut.open(file, std::ofstream::out);

#ifdef DEBUG_MODE
        cout << "Gravando os offsets:" << endl;
#endif
        for (int i = 0; i < 3; ++i) {
            phOffsetOut << ps[i].getOffset() << " ";

#ifdef DEBUG_MODE
            cout << "D(" << i << ")_off=" << ps[i].getOffset() << endl;
#endif
        }

        phOffsetOut.close();

        return *this;
    }

    TAcs& writePhases() {
        for (int i = 0; i < 3; ++i){
		ps[i].writeDat();
	}
#ifdef ARCH_RASP
        for (int j = 0; j < 8; ++j) {
            gpioWrite(mapPort0[j], ps[0].dat[j]); /* on */
            gpioWrite(mapPort1[j], ps[1].dat[j]); /* on */
            gpioWrite(mapPort2[j], ps[2].dat[j]); /* on */
        }
#endif
        return *this;
    }

    const short unsigned int mapPort0[8] = {22, 10, 9, 11, 0, 27, 17, 4};
    const short unsigned int mapPort1[8] = {19, 26, 21, 20, 16, 13, 6, 5};
    const short unsigned int mapPort2[8] = {8, 25, 24, 23, 18, 7, 1, 12};
};


#endif /* TACS_H */

