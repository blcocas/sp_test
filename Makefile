dht : dht.o
	gcc -o dht dht.o -l wiringPi

dhtTolcd : dhtTolcd.o
	gcc -o dhtTolcd dhtTolcd.o -l wiringPi

dht.o : dht.c
	gcc -c dht.c

dhtTolcd.o : dhtTolcd.c
	gcc -c dhtTolcd.c

clean : 
	rm  *.o dht dhtTolcd
