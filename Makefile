all:
	g++ -c Serial.cpp -o Serial.o 
	g++ -c Attenuator.cpp -o Attenuator.o 
	g++ main.cpp Serial.o Attenuator.o -o main 
	
run: 
	./main
	
clean:
	rm Attenuator.o
	rm Serial.o