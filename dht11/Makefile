dht : dht.o
	gcc -o dht dht.o -l wiringPi

dht.o : dht.c
	gcc -c dht.c

clean : 
	rm  *.o dht
