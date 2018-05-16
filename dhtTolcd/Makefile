
dhtTolcd : dhtTolcd.o
	gcc -o dhtTolcd dhtTolcd.o -lwiringPi -lwiringPiDev

dhtTolcd.o : dhtTolcd.c
	gcc -c dhtTolcd.c

clean : 
	rm  *.o dht dhtTolcd
