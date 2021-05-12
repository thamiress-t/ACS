
LIBS_ARC=-lpigpio -lrt -lpthread
LIBS_PC=-lrt -lpthread
LIBS=$(LIBS_PC)

all:
	g++ -c ToolBoxAndUtilities.cpp -o ToolBoxAndUtilities.o
	g++ -c Serial.cpp -o Serial.o
	g++ -c Attenuator.cpp -o Attenuator.o 
	g++ -c TTable.cpp -o TTable.o
	g++ -c TPhShifter.cpp -o TPhShifter.o
	g++ -c TCommand.cpp -o TCommand.o
	g++ -c TAcs.cpp -o TAcs.o 
	g++ main.cpp TCommand.o TPhShifter.o TAcs.o TTable.o ToolBoxAndUtilities.o Serial.o Attenuator.o -o acs -lrt -lpthread -lpigpio

run: 
	./main
	
clean:
	rm -f ToolBoxAndUtilities.o
	rm -f Attenuator.o
	rm -f Serial.o
	rm -f TTable.o
	rm -f TPhShifter.o
	rm -f TAcs.o
	rm -f TCommand.o
	rm -f main.o