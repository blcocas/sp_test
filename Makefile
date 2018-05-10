dht : dht.o
	gcc -o dht dht.o

dhtTolcd : dhtTolcd.o
	gcc -o dhtTolcd dhtTolcd.o

dht.o : dht.c
	gcc -c dht.c

dhtTolcd.o : dhtTolcd.c
	gcc -c dhtTolcd.c

clean : 
	rm  *. dht dhtTolcd
