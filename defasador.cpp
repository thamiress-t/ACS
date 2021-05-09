
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

   gcc -o defasador defasador.c -lpigpio -lrt -lpthread

   sudo ./defasador
*/


#define ARCH_RASP 1
#define DEBUG_MODE 1

#ifdef ARCH_RASP
#include <pigpio.h>
#endif

using namespace std;


class TPhShifter {
protected:
    float step = -1.4606;
    float offset = 0;
    unsigned short phaseInt = 0;

public:
    unsigned short dat[8];

    TPhShifter(const float off = 0) {
        this->offset = off;
        this->step = -1.4606;
        for (int i = 0; i < 8; ++i) dat[i] = 0;
    }

    float int2Deg(const int i) {
        return ((i&0xff) * step + offset);
    }

    float deg2Int(const float d) {
        int x = ((d - offset) / step);
        return (x&0xff);
    }

    TPhShifter& setOffset(const float offset) {
        this->offset = offset; //em graus
        return *this;
    }

    float getOffset() const {
        return offset;
    }

    TPhShifter& setPhaseDeg(const float d) {
        phaseInt = deg2Int(d);
        return *this;
    }

    TPhShifter& setPhaseInt(const unsigned int i) {
        phaseInt = i&0xff;
        return *this;
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

    std::string str() {
        stringstream ss;
        ss<< "[" << phaseInt << "] ";
        for (int i = 0; i < 8; ++i) {
            ss << dat[i] << " ";
        }
        return ss.str();
    }
};

class CAT2 {
public:
    TPhShifter ps[3];

    CAT2() {
        

#ifdef ARCH_RASP
   if (gpioInitialise() < 0)
   {
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
    
    ~CAT2(){
#ifdef ARCH_RASP
        /* Stop DMA, release resources */
        gpioTerminate();
#endif
    }

    CAT2& loadParameters(std::string file = "offset.txt") {
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

    CAT2& saveParameters(std::string file = "offset.txt") {

        ofstream phOffsetOut;
        cout << "Criando arquivo " << file << endl;
        phOffsetOut.open(file, std::ofstream::out);

#ifdef DEBUG_MODE
        cout << "Gravando os offsets:" << endl;
#endif
        for (int i = 0; i < 3; ++i)
        {
            phOffsetOut << ps[i].getOffset() << " ";
            
#ifdef DEBUG_MODE
                cout << "D(" << i << ")_off=" << ps[i].getOffset() << endl;
#endif
        }

        phOffsetOut.close();

	return *this;
    }

    CAT2& writePhases() {
        for (int i = 0; i < 3; ++i)
            ps[i].writeDat();
#ifdef ARCH_RASP
        for (int j=0;j<8;++j)
        {
            gpioWrite(mapPort0[j], ps[0].dat[j]); /* on */
            gpioWrite(mapPort1[j], ps[1].dat[j]); /* on */
            gpioWrite(mapPort2[j], ps[2].dat[j]); /* on */
        }
#endif
       return *this;
    }

    const short unsigned int mapPort0[8]={22,10,9,11,0,27,17,4};
    const short unsigned int mapPort1[8]={19,26,21,20,16,13,6,5};
    const short unsigned int mapPort2[8]={8,25,24,23,18,7,1,12};


};

/*
 * 
 */
int main(int argc, char** argv) {
    cout << argv[2];
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

