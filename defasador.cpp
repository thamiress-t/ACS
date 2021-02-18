/*
 * To change this license header, choose License Headers in Project Properties.
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   defasador.cpp
 * Author: Daniel de Filgueiras Gomes
 *
 * Created on 9 de Setembro de 2020, 06:07
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string.h>

/*
 * No Raspberry:
 Habilitar -> ARCH_RASP
 * defasador.cpp

   g++ -o defasador defasador.cpp -lpigpio -lrt -lpthread

   sudo ./defasador
*/


#define ARCH_RASP 1
#define DEBUG_MODE 1

#ifdef ARCH_RASP
#include <pigpio.h>
#endif

#include <PhShifter.h>

using namespace std;

TPhShifter::TPhShifter(const float off = 0) {
    this->offset = off;
    this->step = -1.4606;
    for (int i = 0; i < 8; ++i) dat[i] = 0;
}

float TPhShifter::int2Deg(const int i) {
    return ((i&0xff) * step + offset);
}

float TPhShifter::deg2Int(const float d) {
    int x = ((d - offset) / step);
    return (x&0xff);
}

TPhShifter& TPhShifter::setOffset(const float offset) {
    this->offset = offset; //em graus
    return *this;
}

float TPhShifter::getOffset() const {
    return offset;
}

TPhShifter& PhShifter::setPhaseDeg(const float d) {
    phaseInt = deg2Int(d);
    return *this;
}

TPhShifter& PhShifter::setPhaseInt(const unsigned int i) {
    phaseInt = i&0xff;
    return *this;
}

std::string TPhShifter::str() {
    stringstream ss;
    ss<< "[" << phaseInt << "] ";
    for (int i = 0; i < 8; ++i) {
        ss << dat[i] << " ";
    }
    return ss.str();
}

TPhShifter& writeDat() {
    dat[0] = (phaseInt & 0x01);
    dat[1] = (phaseInt>>1 & 0x01);
    dat[2] = (phaseInt>>2 & 0x01);
    dat[3] = (phaseInt>>3 & 0x01);
    dat[4] = (phaseInt>>4 & 0x01);
    dat[5] = (phaseInt>>5 & 0x01);
    dat[6] = (phaseInt>>6 & 0x01);
    dat[7] = (phaseInt>>7 & 0x01);
    
#ifdef DEBUG_MODE
    cout << "***************" << endl;
    cout << str();
    cout << endl << "***************" << endl;
#endif
    return *this;
}


/*
 * 
 */
int main(int argc, char** argv) {

    CAT2 cat;
    cat.loadParameters();

    if (argc == 5) {

        int N = 3;
        
        if (strcmp(argv[1], "-int")==0) {
            int x;
            for (int i = 0; i < N; ++i) {
                stringstream ss;
                ss << argv[i + 2];
                ss >> x;
                cat.ps[i].setPhaseInt(x);
            }
            cout << endl;
        } else
            if (strcmp(argv[1], "-deg")==0) {
            float x;
            for (int i = 0; i < N; ++i) {
                stringstream ss;
                ss << argv[i + 2];
                ss >> x;
                cat.ps[i].setPhaseDeg(x);
            }
            cout << endl;
        }
            else
                cout << "Sintaxe inválida!" << endl;
        cout << flush;
        
        cat.writePhases();
        cat.saveParameters();

    }
    else
        if(argc==2 && strcmp(argv[1],"-help")==0) {
            cout << "Opções:" << endl;
            cout << "Fases digitais: defasador -int <0-255> <0-255> <0-255>" << endl;
            cout << "Fases em graus: defasador -deg <0-255> <0-255> <0-255>" << endl;
        }



    return 0;
}

